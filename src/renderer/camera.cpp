#include "./camera.hpp"

#include "./render.hpp"

#include <cstdlib>

void camera_process(Camera* camera, Event* event, f32 delta_time) {

	Key key = event->key;
	switch (key) {
	case Key::W: {
		camera->position += camera->basis.z * (CAMERA_SPEED * delta_time);
		break;
	}
	case Key::S: {
		camera->position -= camera->basis.z * (CAMERA_SPEED * delta_time);
		break;
	}
	case Key::A: {
		camera->position -= camera->basis.x * (CAMERA_SPEED * delta_time);
		break;
	}
	case Key::D: {
		camera->position += camera->basis.x * (CAMERA_SPEED * delta_time);
		break;
	}
	}

	static Vector2 old_mouse_position = event->mouse_position;

	Vector2 mouse_direction = event->mouse_position - old_mouse_position;
	old_mouse_position = event->mouse_position;

	if (mouse_direction.x != 0 || mouse_direction.y != 0) {
		mouse_direction = Math::normalize(mouse_direction);
	}

	Vector2 mouse_velocity = mouse_direction * (CAMERA_ROTATION_SPEED * delta_time);
	if (key != Key::CONTROL) {
		mouse_velocity = { 0, 0 };
	}

	camera->rotation.y += mouse_velocity.x;
	camera->rotation.x += mouse_velocity.y;

	camera->rotation.x = Math::clamp(camera->rotation.x, -89.0f, 89.0f);

	f32 x_rad = Math::to_radians(camera->rotation.x);
	f32 y_rad = Math::to_radians(camera->rotation.y);

	camera->basis.z_axis.x = sinf(y_rad) * cosf(x_rad);
	camera->basis.z_axis.y = -sinf(x_rad);
	camera->basis.z_axis.z = cosf(y_rad) * cosf(x_rad);
	camera->basis.z_axis = Math::normalize(camera->basis.z_axis);

	Vector3 up_vector = { 0, 1, 0 };
	camera->basis.x_axis = Math::cross(up_vector, camera->basis.z_axis);
	camera->basis.x_axis = Math::normalize(camera->basis.x_axis);

	camera->basis.y_axis = Math::cross(camera->basis.z_axis, camera->basis.x_axis);
	camera->basis.y_axis = Math::normalize(camera->basis.y_axis);
}
