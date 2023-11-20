#include "Editor.h"
#include "Scene.h"

namespace nc {
	void Editor::proccessGui(Scene* scene) {
		// show resources
		ImGui::Begin("Resources");
		auto resources = GET_RESOURCES(Resource);
		for (auto& resource : resources) {
			if (ImGui::Selectable(resource->name.c_str(), resource.get() == m_selected)) {
				m_selected = resource.get();
			}
		}
		ImGui::End();

		// show scene
		ImGui::Begin("Scene");
		scene->processGui();
		ImGui::Separator();
		// show actors
		for (auto& actor : scene->m_actors) {
			if (ImGui::Selectable(actor->name.c_str(), actor.get() == m_selected)) {
				m_selected = actor.get();
			}
		}
		
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
