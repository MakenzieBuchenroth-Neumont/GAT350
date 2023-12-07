#include "Editor.h"
#include "Scene.h"
#include "Components/CameraComponent.h"

namespace nc {
	void Editor::update() {
		if (ImGui::IsKeyPressed(ImGuiKey_GraveAccent)) m_active = !m_active;
	}

	void Editor::proccessGui(Scene* scene) {
		if (!m_active) return;

		// show resources
		/*
		auto resources = GET_RESOURCES(Resource);
		for (auto& resource : resources) {
			if (ImGui::Selectable(resource->name.c_str(), resource.get() == m_selected)) {
				m_selected = resource.get();
			}
		}
		*/

		ImGui::Begin("Resources");
		const char* types[] = { "Texture", "Model", "Material", "Shader"};
		ImGui::Combo("Type", (int*)(&type), types, 4);
		if (type == TEXTURE) {
			auto resources = GET_RESOURCES(Texture);
			for (auto& resource : resources) {
				if (ImGui::Selectable(resource->name.c_str(), resource.get() == m_selected)) {
					m_selected = resource.get();
				}
			}
		}
		if (type == MODEL) {
			auto resources = GET_RESOURCES(Model);
			for (auto& resource : resources) {
				if (ImGui::Selectable(resource->name.c_str(), resource.get() == m_selected)) {
					m_selected = resource.get();
				}
			}
		}
		if (type == MATERIAL) {
			auto resources = GET_RESOURCES(Material);
			for (auto& resource : resources) {
				if (ImGui::Selectable(resource->name.c_str(), resource.get() == m_selected)) {
					m_selected = resource.get();
				}
			}
		}
		if (type == SHADER) {
			auto resources = GET_RESOURCES(Shader);
			for (auto& resource : resources) {
				if (ImGui::Selectable(resource->name.c_str(), resource.get() == m_selected)) {
					m_selected = resource.get();
				}
			}
		}
		ImGui::End();

		// show scene
		ImGui::Begin("Scene");
		scene->processGui();
		ImGui::Separator();
		// show actors
		ImGui::BeginChild("Actors");
		// actor controls
		if (ImGui::BeginPopupContextWindow()) {
			auto cameras = scene->GetComponents<CameraComponent>();
			auto camera = (!cameras.empty()) ? cameras[0] : nullptr;
			if (ImGui::MenuItem("Create Empty")) {
				auto actor = CREATE_CLASS(Actor);
				actor->name = CreateUnique(actor->GetClassName());
				if (camera) {
					actor->transform.position = camera->m_owner->transform.position + camera->m_owner->transform.Forward() * 3.0f;
				}
				actor->Initialize();
				m_selected = actor.get();
				scene->Add(std::move(actor));
			}
			if (ImGui::MenuItem("Sphere")) {
				auto actor = CREATE_CLASS_BASE(Actor, "Sphere");
				actor->name = CreateUnique(actor->GetClassName());
				if (camera) {
					actor->transform.position = camera->m_owner->transform.position + camera->m_owner->transform.Forward() * 3.0f;
				}
				actor->Initialize();
				m_selected = actor.get();
				scene->Add(std::move(actor));
			}
			if (ImGui::MenuItem("Cube")) {
				auto actor = CREATE_CLASS_BASE(Actor, "Cube");
				actor->name = CreateUnique(actor->GetClassName());
				if (camera) {
					actor->transform.position = camera->m_owner->transform.position + camera->m_owner->transform.Forward() * 3.0f;
				}
					actor->Initialize();
					m_selected = actor.get();
					scene->Add(std::move(actor));
			}
			if (ImGui::MenuItem("Camera")) {
				auto actor = CREATE_CLASS_BASE(Actor, "Camera");
				actor->name = CreateUnique(actor->GetClassName());
				if (camera) {
					actor->transform.position = camera->m_owner->transform.position + camera->m_owner->transform.Forward() * 3.0f;
				}
					actor->Initialize();
					m_selected = actor.get();
					scene->Add(std::move(actor));
			}
			if (ImGui::MenuItem("Light")) {
				auto actor = CREATE_CLASS_BASE(Actor, "Light");
				actor->name = CreateUnique(actor->GetClassName());
				if (camera) {
					actor->transform.position = camera->m_owner->transform.position + camera->m_owner->transform.Forward() * 3.0f;
				}
				actor->Initialize();
				m_selected = actor.get();
				scene->Add(std::move(actor));
			}
			ImGui::EndPopup();
			}
			for (auto& actor : scene->m_actors) {
				if (ImGui::Selectable(actor->name.c_str(), actor.get() == m_selected)) {
					m_selected = actor.get();
				}
			}
			ImGui::EndChild();
			ImGui::End();

			// show inspector
			ImGui::Begin("Inspector");
				if (m_selected) {
					m_selected->processGui();
					// delete selected actor
					if (ImGui::IsKeyPressed(ImGuiKey_Delete)) {
						auto actor = dynamic_cast<Actor*>(m_selected);
						if (actor) {
							scene->Remove(actor);
							m_selected = nullptr;
						}
					}
				}
			ImGui::End();
		}
	}
