#include <ctime>
#include "application.h"
#include "types.h"
#include "mesh.h"
#include <iostream>


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
            Engine::Math::Vec3::UNITSCALE * static_cast<real>(3.0)
        }, renderer.device
    );

    quad = Engine::Resources::Mesh::GenerateQuad(
        Engine::Math::Transform{
            Engine::Math::Vec3 { 10, 0, 0 },
            Engine::Math::Quaternion::ZERO,
            Engine::Math::Vec3::UNITSCALE * static_cast<real>(3.0)
        }, renderer.device
    );

    this->currentState = ApplicationState::Running;
}

void Application::Run()
{
    Engine::Math::Vec3 standardScale = cube.transform.scale;
    Engine::Math::Vec3 tempScale = cube.transform.scale;
    bool inverse = false;
    real scale = static_cast<real>(1.0);

    while (this->currentState == ApplicationState::Running)
    {
        if (!window.MessagePump())
        {
            this->currentState = ApplicationState::Stopped;
        }
        float time = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - t_start).count();
        renderer.BeginScene();
        renderer.RenderCube(Engine::Math::Quaternion::FromAngleAxis(static_cast<real>(time * 360.0f), Engine::Math::Vec3::UNITX - Engine::Math::Vec3::UNITY), cube);
        renderer.RenderQuad(quad);
        renderer.EndScene();

        if (time >= 1.0f)
        {
            t_start = std::chrono::high_resolution_clock::now();
            inverse = !inverse;
            tempScale = cube.transform.scale;
        }
        if (inverse)
        {
            scale = static_cast<real>(0.3);
        }
        else
        {
            scale = static_cast<real>(2.0);
        }

        cube.transform.scale = tempScale.Lerp(standardScale * scale, time);
    }

}
void Application::Shutdown()
{
    renderer.Shutdown();
}

