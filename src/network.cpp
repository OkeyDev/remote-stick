#include "car_code/network.h"
#include "car_code/models.h"
#include "car_code/movement.h"
#include "godot_cpp/core/print_string.hpp"
#include "godot_cpp/variant/variant.hpp"

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <gdextension_interface.h>
#include <cstdint>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include <cerrno>
#include <chrono>
#include <cstring>

#define PORT 8080
#define BUFFER_SIZE 1024
#define CLIENT_TIMEOUT_MS 2000
using namespace godot;

static int serverfd = -1;
static char buffer[BUFFER_SIZE] = {};

static struct sockaddr_in last_client_addr;
static bool has_active_client = false;
static uint64_t last_packet_time_ms = 0;

static uint64_t get_current_time_ms() {
	using namespace std::chrono;
	return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

static void set_nonblocking(int s) {
	int flags = fcntl(s, F_GETFL, 0);
	if (flags >= 0) {
		fcntl(s, F_SETFL, flags | O_NONBLOCK);
	}
}

char MAGIC_WORD[] = "move";

struct MovementPacket 
{
	MovementState m_state;
};

int net_init() {
	serverfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (serverfd < 0) {
		print_error(vformat("socket: error %d", errno));
		return -1;
	}

	int opt = 1;
	if (setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		print_error(vformat("setsockopt: error %d", errno));
		close(serverfd);
		serverfd = -1;
		return -1;
	}

	struct sockaddr_in servaddr;
	std::memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);

	if (bind(serverfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
		print_error(vformat("bind: error %d", errno));
		close(serverfd);
		serverfd = -1;
		return -1;
	}

	//set_nonblocking(serverfd);

	has_active_client = false;
	last_packet_time_ms = 0;

	print_line(vformat("UDP Server started on port: %d (socket: %d)", PORT, serverfd));
	return 0;
}

void net_update() {
	if (serverfd < 0) {
		return;
	}

	struct sockaddr_in client_addr;
	socklen_t addrlen = sizeof(client_addr);

	while (true) {
		ssize_t bytes_read = recvfrom(serverfd, buffer, BUFFER_SIZE - 1, 0,
				(struct sockaddr *)&client_addr, &addrlen);

		if (bytes_read < 0) {
			// Очередь пуста — выходим без блокировки потока
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				continue;
			}
			if (errno != EINTR) {
				print_error(vformat("recvfrom error: %d", errno));
			}
			continue;
		}

		buffer[bytes_read] = '\0';

		last_client_addr = client_addr;
		has_active_client = true;
		last_packet_time_ms = get_current_time_ms();

		char client_ip[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, sizeof(client_ip));
		print_line(vformat("Value received from %s:%d: %s", client_ip, ntohs(client_addr.sin_port), buffer));

		const char *response = "OK\n";
		sendto(serverfd, response, std::strlen(response), 0,
				(const struct sockaddr *)&client_addr, addrlen);

		if (strncmp(buffer, MAGIC_WORD, sizeof(MAGIC_WORD) - 1) == 0) {
			MovementState state = (MovementState)atoi(buffer + sizeof(MAGIC_WORD) - 1);
			print_line(vformat("good packet: %d", (int)state));
			movement_set_state(state);
		}
		else {
			print_line("failed to compare magic word");
		}
	}

	if (has_active_client && (get_current_time_ms() - last_packet_time_ms > CLIENT_TIMEOUT_MS)) {
		has_active_client = false;
		print_line("UDP client connection timed out");
	}
}

int net_connection_active() {
	return has_active_client ? 1 : 0;
}

MovementState net_get_current_status() {
	return STOP;
}

void net_stop() {
	if (serverfd >= 0) {
		close(serverfd);
		serverfd = -1;
		has_active_client = false;
		print_line("UDP Server stopped");
	}
}
