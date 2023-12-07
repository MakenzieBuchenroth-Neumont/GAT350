#include "World08.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/color_space.hpp>
#include "Core/Math/MathUtils.h"

#include <iostream>


namespace nc
{
    bool World08::Initialize() {
        
        m_scene = std::make_unique<Scene>();
        m_scene->Load("Scenes/sceneFinal.json");
        m_scene->Initialize();

        m_editor = std::make_unique<Editor>();

        // create depth texture
        auto texture = std::make_shared<Texture>();
        texture->CreateDepthTexture(1024, 1024);
        ADD_RESOURCE("depth_texture", texture);

        // create depth buffer
        auto framebuffer = std::make_shared<Framebuffer>();
        framebuffer->CreateDepthBuffer(texture);
        ADD_RESOURCE("depth_buffer", framebuffer);

        // set depth texture to debug sprite
        auto material = GET_RESOURCE(Material, "materials/sprite.mtrl");
        if (material) {
            material->albedoTexture = texture;
        }

        auto materials = GET_RESOURCES(Material);
        for (auto material : materials)
        {
            material->depthTexture = texture;
        }

        return true;
    }

    void World08::Shutdown() {
    }

    void World08::Update(float dt) {
        m_time += dt;

        ENGINE.GetSystem<Gui>()->BeginFrame();

        m_scene->Update(dt);

        m_editor->update();
        m_editor->proccessGui(m_scene.get());

        ImGui::Begin("Cel");
        ImGui::DragInt("Levels", &celLevels);
        ImGui::DragFloat("Cel Specular Cutoff", &celSpecularCutoff);
        ImGui::DragFloat("Cel Outline", &celOutline);
        ImGui::End();

        auto program = GET_RESOURCE(Program, "shaders/cel.prog");
        if (program) {
            program->Use();
            program->SetUniform("celLevels", celLevels);
            program->SetUniform("celSpecularCutoff", celSpecularCutoff);
            program->SetUniform("celOutline", celOutline);
        }

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World08::Draw(Renderer& renderer) {
        // *** PASS 1 ***
        auto framebuffer = GET_RESOURCE(Framebuffer, "depth_buffer");
        renderer.SetViewport(framebuffer->GetSize().x, framebuffer->GetSize().y);
        framebuffer->Bind();

        renderer.ClearDepth();
        auto program = GET_RESOURCE(Program, "shaders/shadow_depth.prog");
        program->Use();

        auto lights = m_scene->GetComponents<LightComponent>();
        for (auto light : lights) {
            if (light->castShadow) {
                //glCullFace(GL_FRONT);
                glm::mat4 shadowMatrix = light->GetShadowMatrix();
                program->SetUniform("shadowVP", shadowMatrix);
            }
        }

        auto models = m_scene->GetComponents<ModelComponent>();
        for (auto model : models) {
            if (model->castShadow) {
                program->SetUniform("model", model->m_owner->transform.GetMatrix());
                model->model->Draw();
            }
        }

        framebuffer->Unbind();
        

        // *** PASS 2 ***
        renderer.ResetViewport();
        renderer.BeginFrame();
        m_scene->Draw(renderer);

        // post-render
        ENGINE.GetSystem<Gui>()->Draw();
        renderer.EndFrame();
    }
}
