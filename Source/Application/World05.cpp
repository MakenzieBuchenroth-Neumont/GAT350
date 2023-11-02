#include "World05.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/color_space.hpp>

#include <iostream>


namespace nc
{
    bool World05::Initialize() {

        std::string one = ToUpper("hellO world");
        std::string two = ToLower("GoODByE");

        std::cout << one << "\n";
        std::cout << two << "\n";

        std::string three = "Hello World";
        std::string four = "Hello World";

        std::cout << IsEqualIgnoreCase(three, four) << "\n";

        std::string five = "Goodbye";
        std::string six = "Hello World";

        std::cout << IsEqualIgnoreCase(five, six) << "\n";

        std::string seven = "Seven";
        std::string eight = "Eight";

        std::cout << CreateUnique(seven) << "\n";
        std::cout << CreateUnique(eight) << "\n";
        

        m_scene = std::make_unique<Scene>();
        m_scene->Load("Scenes/scene.json");
        m_scene->Initialize();

        // CAMERA
        //{
        //    auto actor = CREATE_CLASS(Actor);
        //    actor->name = "camera1";
        //    actor->transform.position = glm::vec3{ 0, 0, 18 };
        //    actor->transform.rotation = glm::vec3{ 0, 180, 0 };



        //    auto cameraComponent = CREATE_CLASS(CameraComponent);
        //    cameraComponent->SetPerspective(70.0f, (float)ENGINE.GetSystem<Renderer>()->GetWidth() / (float)ENGINE.GetSystem<Renderer>()->GetHeight(), 0.1f, 100.0f);
        //    actor->AddComponent(std::move(cameraComponent));

        //    m_scene->Add(std::move(actor));
        //}

        /*
        {
            auto actor = CREATE_CLASS(Actor);
            actor->name = "actor1";
            actor->transform.position = glm::vec3{ 0, 0, 0 };
            auto modelComponent = CREATE_CLASS(ModelComponent);
            modelComponent->model = std::make_shared<Model>();
            modelComponent->model->SetMaterial(GET_RESOURCE(Material, "materials/squirrel.mtrl"));
            modelComponent->model->Load("models/squirrel.glb", glm::vec3{ 0, -0.7f, 0 }, glm::vec3{ 0 }, glm::vec3{ 0.4f });
            actor->AddComponent(std::move(modelComponent));
            m_scene->Add(std::move(actor));
        }

        {
            auto actor = CREATE_CLASS(Actor);
            actor->name = "actor2";
            actor->transform.position = glm::vec3{ 0, 0, 0 };
            auto modelComponent = CREATE_CLASS(ModelComponent);
            modelComponent->model = std::make_shared<Model>();
            modelComponent->model->SetMaterial(GET_RESOURCE(Material, "materials/foxy.mtrl"));
            modelComponent->model->Load("models/foxy.obj", glm::vec3{ 0, 0, 0 }, glm::vec3{ 0 }, glm::vec3{ 0 });
            actor->AddComponent(std::move(modelComponent));
            m_scene->Add(std::move(actor));
        }
        */

        {
            auto actor = CREATE_CLASS(Actor);
            actor->name = "light1";
            actor->transform.position = glm::vec3{ 3, 3, 3 };
            auto lightComponent = CREATE_CLASS(LightComponent);
            lightComponent->type = LightComponent::eType::Point;
            lightComponent->color = glm::rgbColor(glm::vec3{ 1,1,1 }); //glm::rgbColor(glm::vec3{ randomf() * 360, 1, 1 });
            lightComponent->intensity = 1;
            lightComponent->range = 20;
            lightComponent->innerAngle = 10.0f;
            lightComponent->outerAngle = 30.0f;
            actor->AddComponent(std::move(lightComponent));
            m_scene->Add(std::move(actor));
        }

        return true;
    }

    void World05::Shutdown() {
    }

    void World05::Update(float dt) {

        ENGINE.GetSystem<Gui>()->BeginFrame();

        m_scene->Update(dt);
        m_scene->processGui();

        auto actor = m_scene->GetActorByName<Actor>("actor1");

        actor->transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? m_speed * +dt * 2 : 0;
        actor->transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? m_speed * -dt * 2 : 0;
        actor->transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? m_speed * +dt * 2 : 0;
        actor->transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? m_speed * -dt * 2 : 0;

        m_time += dt;

        auto material = actor->GetComponent<ModelComponent>()->model->GetMaterial();

        material->processGui();
        material->Bind();

        material = GET_RESOURCE(Material, "materials/refraction.mtrl");
        if (material) {
            ImGui::Begin("Refraction");
            //m_refraction = 1.0f + std::fabs(std::sin(m_time * 0.1f));
            ImGui::DragFloat("IOR", &m_refraction, 0.01f, 1, 3);
            auto program = material->GetProgram();
            program->Use();
            program->SetUniform("ior", m_refraction);
            ImGui::End();
        }

        // add light vector and gui


        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World05::Draw(Renderer& renderer) {
        // pre-render
        renderer.BeginFrame();
        // render
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        m_scene->Draw(renderer);
        ENGINE.GetSystem<Gui>()->Draw();
        // post-render
        renderer.EndFrame();
    }
}
