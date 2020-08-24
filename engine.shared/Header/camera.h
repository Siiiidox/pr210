#pragma once
#include "transform.h"
class Camera
{
public:
	/*!
	*	@brief Transform of the Camera
	*/
	Engine::Math::Transform transform;
	/*!
	*	@brief FieldOfView of the Camera
	*/
	float h_fov = 75.f;
	/*!
	*	@brief Nearplane of the Camera
	*/
	float near_plane = 0.01f;
	/*!
	*	@brief Farplane of the Camera
	*/
	float far_plane = 10000.f;
	/*!
	*	@brief enum for the Type of the Camera
	*/
	enum CameraType
	{
		Perspective,
		Orthographic
	} cameraType = Perspective;

};