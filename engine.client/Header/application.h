#pragma once
#include "applicationWindow.h"
#include "d3d11renderer.h"
#include <chrono>
#include "camera.h"
#include "mesh.h"

class Application
{

private:
	/*!
	*	@brief variable to store the application window
	*/
	ApplicationWindow window;
	/*!
	*	@brief variable to store the D3D11Renderer
	*/
	Engine::Graphics::D3D11Renderer renderer;
	/*!
	*	@brief variable to store the camera
	*/
	Camera camera;
	/*!
	*	@brief variable to store the cube object
	*/
	Engine::Resources::MeshObject cube;
	/*!
	*	@brief variable to store the quad object
	*/
	Engine::Resources::MeshObject quad;
	/*!
	*	@brief enum which stores the application State
	*/
	enum class ApplicationState
	{
		Started,
		Running,
		Stopped
	} currentState = ApplicationState::Started;
	/*!
	*	@brief the time when the Application started
	*	In Use to rotate the Cube
	*/
	std::chrono::steady_clock::time_point t_start = std::chrono::high_resolution_clock::now();
public:
	/*!
	*	@brief Initialize the application
	*	@return void
	*/
	void Init();
	/*!
	*	@brief Run in a while the application 
	*	@return void
	*/
	void Run();
	/*!
	*	@brief shutdown the application
	*	@return void
	*/
	void Shutdown();

};