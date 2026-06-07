#include "Object.h"
#include "CommonVari.h"

void Object::Move(int move_x, int move_y) {
	x += move_x;
	y += move_y;
}

void Object::MoveDirection(int z, direction dire) {
	if (dire == down)
		Move(0, z);
	else if (dire == left)
		Move(-z, 0);
	else if (dire == right)
		Move(z, 0);
	else if (dire == up)
		Move(0, -z);
}

void Object::Set(int set_x, int set_y) {
	x = set_x;
	y = set_y;
}

void Object::Draw() {
	DrawGraph(x, y, image_handle, TRUE);
}