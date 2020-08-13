#pragma once
#include "transform.h"
class Camera
{
public:
	Engine::Math::Transform transform;
	float h_fov = 75.f;
	float near_plane = 0.01f;
	float far_plane = 10000.f;
	enum CameraType
	{
		Perspective,
		Orthographic
	} cameraType = Perspective;

};