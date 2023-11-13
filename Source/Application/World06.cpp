#include "World06.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/color_space.hpp>

#include <iostream>


namespace nc
{
    bool World06::Initialize() {
         m_scene = std::make_unique<Scene>();
        m_scene->Load("Scenes/sceneFramebuffer.json");
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

    void World06::Shutdown() {
    }

    void World06::Update(float dt) {

        ENGINE.GetSystem<Gui>()->BeginFrame();

        m_scene->Update(dt);
        m_scene->processGui();

        // set post process gui
        ImGui::Begin("Post Process");
        ImGui::SliderFloat("Blend", &m_blend, 0, 1);
        ImGui::ColorEdit3("Tint Color", glm::value_ptr(m_colortint));
        ImGui::SliderFloat("Bloom Threshold", &m_bloomThreshold, 0, 1);
        ImGui::SliderFloat("Bloom Intensity", &m_bloomIntensity, 0, 1);

        bool effect = m_params & INVERT_MASK;
        if (ImGui::Checkbox("Invert", &effect)) {
            (effect) ? m_params |= INVERT_MASK : m_params &= ~INVERT_MASK;
        }
        effect = m_params & GRAYSCALE_MASK;
        if (ImGui::Checkbox("Grayscale", &effect)) {
            (effect) ? m_params |= GRAYSCALE_MASK : m_params &= ~GRAYSCALE_MASK;
        }
        effect = m_params & COLORTINT_MASK;
        if (ImGui::Checkbox("Color Tint", &effect)) {
            (effect) ? m_params |= COLORTINT_MASK : m_params &= ~COLORTINT_MASK;
        }
        effect = m_params & GRAIN_MASK;
        if (ImGui::Checkbox("Grain", &effect)) {
            (effect) ? m_params |= GRAIN_MASK : m_params &= ~GRAIN_MASK;
        }
        effect = m_params & SCANLINE_MASK;
        if (ImGui::Checkbox("Scanline", &effect)) {
            (effect) ? m_params |= SCANLINE_MASK : m_params &= ~SCANLINE_MASK;
        }
        effect = m_params & BLOOM_MASK;
        if (ImGui::Checkbox("Bloom", &effect)) {
            (effect) ? m_params |= BLOOM_MASK : m_params &= ~BLOOM_MASK;
        }
        ImGui::End();

        // set post process shader
        auto program = GET_RESOURCE(Program, "shaders/post_process.prog");
        if (program) {
            program->Use();
            program->SetUniform("blend", m_blend);
            program->SetUniform("params", m_params);
            program->SetUniform("tintcolor", m_colortint);
            program->SetUniform("time", m_time);
            program->SetUniform("bloomThreshold", m_bloomThreshold);
            program->SetUniform("bloomIntensity", m_bloomIntensity);
        }

        m_time += dt;

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World06::Draw(Renderer& renderer) {
        // *** PASS 1 ***
        m_scene->GetActorByName("postprocess")->active = false;

        auto framebuffer = GET_RESOURCE(Framebuffer, "fb");
        renderer.SetViewport(framebuffer->GetSize().x, framebuffer->GetSize().y);
        framebuffer->Bind();

        renderer.BeginFrame({1, 0, 0.5});
        m_scene->Draw(renderer);

        framebuffer->Unbind();

        // *** PASS 2 ***
        m_scene->GetActorByName("postprocess")->active = true;
        renderer.ResetViewport();
        renderer.BeginFrame();
        m_scene->GetActorByName("postprocess")->Draw(renderer);

        // post-render
        ENGINE.GetSystem<Gui>()->Draw();
        renderer.EndFrame();
    }
}
