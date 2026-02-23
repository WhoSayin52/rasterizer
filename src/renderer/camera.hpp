#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <core/core.hpp>

constexpr f32 CAMERA_SPEED = 10.0f;
constexpr f32 CAMERA_ROTATION_SPEED = 100.0f;

struct Event;

struct Camera {
	Vector3 position;
	Vector3 rotation;
	Matrix3 basis;

	f32 fov; // field of view in radians;
	f32 near; // distance between camera and viewport
	f32 far;
	f32 aspect_ratio;
	Vector2 viewport;
};

void camera_process(Camera* camera, Event* event, f32 delta_time);

#endif // CAMERA_HPP
