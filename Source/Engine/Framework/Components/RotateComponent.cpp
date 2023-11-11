#include "RotateComponent.h"
#include "Framework/Actor.h"
#include "Framework/Engine.h"

namespace nc
{
	CLASS_DEFINITION(RotateComponent)

		bool RotateComponent::Initialize()
	{
		

		return true;
	}

	void RotateComponent::Update(float dt)
	{
		glm::quat rotation = EulerToQuaternion(euler * dt);
		m_owner->transform.rotation = m_owner->transform.rotation * rotation;
	}

	void RotateComponent::processGui()
	{
		ImGui::DragFloat3("Rotate", glm::value_ptr(euler));
	}


	void RotateComponent::Read(const json_t& value)
	{
		READ_DATA(value, euler);
	}
}