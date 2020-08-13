#include <ctime>
#include "application.h"
#include "types.h"
#include "mesh.h"


void Application::Init()
{
	window.Init("PRO210 Engine", 1280, 720);
	if (!renderer.Init(window))
	{
		return;
	}
	camera.transform.position.z += 10.0;
	renderer.SetActiveCamera(camera);

	cube = Engine::Resources::Mesh::GenerateCube(
		Engine::Math::Transform{
		Engine::Math::Vec3::ZERO,
		Engine::Math::Quaternion::ZERO,
		/*Engine::Math::Quaternion::FromAngleAxis(static_cast<real>(0 * 180.0f),
		Engine::Math::Vec3::UNITY - Engine::Math::Vec3::UNITX),*/
		Engine::Math::Vec3::UNITSCALE * static_cast<real>(3.0) },
		renderer.device
		);

	this->currentState = ApplicationState::Running;
}

void Application::Run()
{
	while (this->currentState == ApplicationState::Running)
	{
		if (!window.MessagePump())
		{
			this->currentState = ApplicationState::Stopped;
		}
		float time = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - t_start).count();
		renderer.BeginScene();
		renderer.RenderCube(Engine::Math::Quaternion::FromAngleAxis(static_cast<real>(time * 180.0f), Engine::Math::Vec3::UNITX - Engine::Math::Vec3::UNITY), cube);
		renderer.EndScene();
	}

}
void Application::Shutdown()
{
	renderer.Shutdown();
	//cube.~Cube();
}

