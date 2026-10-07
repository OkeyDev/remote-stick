extends Node


func _ready() -> void:
	var example := Example2Class.new()
	example.print_type(example)
