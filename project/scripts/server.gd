extends Node


# Called when the node enters the scene tree for the first time.
var file = NetworkClass.new()
func _ready() -> void:
	print("Init started")
	file.init()
	print("Init ended")

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	# print("Update started")
	file.update()
	# print("Update ended")
