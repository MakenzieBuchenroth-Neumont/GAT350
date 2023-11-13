#include "World07.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/color_space.hpp>

#include <iostream>


namespace nc
{
    bool World07::Initialize() {
         m_scene = std::make_unique<Scene>();
        m_scene->Load("Scenes/sceneShadow.json");
        m_scene->Initialize();

        auto texture = std::make_shared<Texture>();
        texture->CreateTexture(1024, 1024);
        ADD_RESOURCE("fb_texture", texture);

        auto framebuffer = std::make_shared<Framebuffer>();
        framebuffer->CreateFramebuffer(texture);
        ADD_RESOURCE("fb", framebuffer);

        auto material = GET_RESOURCE(Material, "materials/postprocess.mtrl");
        if (material) {
            material->albedoTexture = texture;
        }


        return true;
    }

    void World07::Shutdown() {
    }

    void World07::Update(float dt) {
        m_time += dt;

        ENGINE.GetSystem<Gui>()->BeginFrame();

        m_scene->Update(dt);
        m_scene->processGui();

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World07::Draw(Renderer& renderer) {
        // *** PASS 1 ***
        /*
        m_scene->GetActorByName("postprocess")->active = false;

        auto framebuffer = GET_RESOURCE(Framebuffer, "fb");
        renderer.SetViewport(framebuffer->GetSize().x, framebuffer->GetSize().y);
        framebuffer->Bind();

        renderer.BeginFrame({0, 0.5, 0.75});
        m_scene->Draw(renderer);

        framebuffer->Unbind();
        */

        // *** PASS 2 ***
        
        renderer.ResetViewport();
        renderer.BeginFrame();
        m_scene->Draw(renderer);

        // post-render
        ENGINE.GetSystem<Gui>()->Draw();
        renderer.EndFrame();
    }
}
