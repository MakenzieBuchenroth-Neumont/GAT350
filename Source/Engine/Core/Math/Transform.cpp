#include "Transform.h"
#include "Renderer/GUI.h"

namespace nc
{
	void Transform::processGui() {
		ImGui::DragFloat3("Position", glm::value_ptr(position), 0.1f);
		ImGui::DragFloat3("Rotation", glm::value_ptr(rotation));
		ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.1f);
	}

	void Transform::Read(const json_t& value)
	{
		//READ_DATA(value, position);
		//READ_DATA(value, rotation);
		//READ_DATA(value, scale);
	}
}
