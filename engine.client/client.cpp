// EXTERNAL INCLUDES
// INTERNAL INCLUDES
#include "client.h"
#include "types.h"
#include "d3d11renderer.h"
#include "transform.h"
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
	camera.transform.position.z += 6.0;
	D3D11Renderer::GetInstance().SetActiveCamera(camera);
	testMesh.GenerateSphere(8, 16);
	this->appState = AppState::Running;
}

void Application::Run()
{
	while(this->appState == AppState::Running)
	{
		D3D11Renderer::GetInstance().BeginScene();
		D3D11Renderer::GetInstance().RenderObject(Engine::Math::Transform{ Vec3::Zero, Quaternion::Zero, Vec3::UnitScale }, testMesh.indexCount, testMesh.vertexBuffer, testMesh.indexBuffer);
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
