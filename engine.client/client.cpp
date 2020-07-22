// EXTERNAL INCLUDES
#include <ctime>
// INTERNAL INCLUDES
#include "client.h"
#include "types.h"
#include "d3d11renderer.h"
#include "transform.h"
#include "mathutils.h"
using namespace Game::Client;
using namespace Engine::Graphics;
using namespace Engine::Math;
void Application::Init()
{
	AnsiString name = "PR210 Engine";
	window.Init(name, 1280, 720);
	window.Show();
	if (!D3D11Renderer::GetInstance().Init(window))
	{
		return;
	}
	camera.transform.position.z += 3.0;
	D3D11Renderer::GetInstance().SetActiveCamera(camera);
	//testMesh.GenerateSphere(32, 64);
	testMesh.GenerateCube();
	this->appState = AppState::Running;
}

void Application::Run()
{
	while(this->appState == AppState::Running)
	{
		float time = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - t_start).count();
		D3D11Renderer::GetInstance().BeginScene();
		D3D11Renderer::GetInstance().RenderObject(Engine::Math::Transform{ Vec3{0, 0, 0.0}, Quaternion::FromAngleAxis(static_cast<real>(time * 180.0f), Vec3::UnitY - Vec3::UnitX), Vec3::UnitScale * 1.0 }, testMesh.indexCount, testMesh.vertexBuffer, testMesh.indexBuffer);
		D3D11Renderer::GetInstance().EndScene();
		if (!window.MessagePump())
		{
			this->appState = AppState::Stopped;
		}
		
	}
}

void Application::Shutdown()
{
	D3D11Renderer::GetInstance().Shutdown();
}
