#include "World04.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include <glm/glm/gtc/type_ptr.hpp>


namespace nc
{
    bool World04::Initialize() {

        auto material = GET_RESOURCE(Material, "materials/foxy.mtrl");
        m_model = std::make_shared<Model>();
        m_model->SetMaterial(material);
        m_model->Load("models/foxy.obj");

        return true;
    }

    void World04::Shutdown() {
    }

    void World04::Update(float dt) {

        ENGINE.GetSystem<Gui>()->BeginFrame();


        auto material = m_model->GetMaterial();
        material->processGui();
        material->Bind();

        ImGui::Begin("Transform");
        ImGui::DragFloat3("Position", &m_transform.position[0], 0.1f);
        ImGui::DragFloat3("Rotation", &m_transform.rotation[0], 0.1f);
        ImGui::DragFloat3("Scale", &m_transform.scale[0], 0.1f);
        ImGui::End();

        ImGui::Begin("Lighting");
        ImGui::DragFloat3("Position", &lightPosition[0], 0.1f);
        ImGui::ColorEdit3("Ambient Color", &ambientColor[0], 0.1f);
        ImGui::ColorEdit3("Diffuse Color", &lightColor[0], 0.1f);
        ImGui::End();

        //m_transform.rotation.z += 180 * dt;

        m_transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? m_speed * +dt * 2 : 0;
        m_transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? m_speed * -dt * 2 : 0;
        m_transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? m_speed * +dt * 2 : 0;
        m_transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? m_speed * -dt * 2 : 0;

        m_time += dt;

        // model matrix
        material->GetProgram()->SetUniform("model", m_transform.GetMatrix());

        // view matrix
        glm::mat4 view = glm::lookAt(glm::vec3{ 0, 0, 3 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 1, 0 });
        material->GetProgram()->SetUniform("view", view);

        // projection matrix
        glm::mat4 projection = glm::perspective(glm::radians(70.0f), 800.0f / 600.0f, 0.01f, 100.0f);
        material->GetProgram()->SetUniform("projection", projection);

        // add light vector and gui
        material->GetProgram()->SetUniform("light.position", lightPosition);
        material->GetProgram()->SetUniform("light.ambientColor", ambientColor);
        material->GetProgram()->SetUniform("light.color", lightColor);


        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World04::Draw(Renderer& renderer) {
        // pre-render
        renderer.BeginFrame();
        // render
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        m_model->Draw(GL_TRIANGLES);
        ENGINE.GetSystem<Gui>()->Draw();
        // post-render
        renderer.EndFrame();
    }
}