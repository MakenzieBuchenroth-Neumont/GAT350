#include "ModelComponent.h"
#include "Framework/Actor.h"
#include "Framework/Resource/ResourceManager.h"

namespace nc
{
	CLASS_DEFINITION(ModelComponent)

	bool ModelComponent::Initialize()
	{
		if (!modelName.empty())
		{
			model = std::make_shared<Model>();
			model = GET_RESOURCE(Model, modelName);
		}
		if (model && !materialName.empty())
		{
			material = GET_RESOURCE(Material, materialName);
		}

		return true;
	}

	void ModelComponent::Update(float dt)
	{
	}

	void ModelComponent::Draw(Renderer& renderer)
	{
		material->Bind();
		material->GetProgram()->SetUniform("model", m_owner->transform.GetMatrix());
		glDepthMask(enableDepth);
		glCullFace(cullFace);
		model->Draw();
	}

	void ModelComponent::Read(const json_t& value)
	{
		READ_DATA(value, modelName);
		READ_DATA(value, materialName);
	
		READ_DATA(value, enableDepth);
		std::string cullFaceName;
		if (READ_NAME_DATA(value, "cullFace", cullFaceName)) {
			if (IsEqualIgnoreCase(cullFaceName, "front")) cullFace = GL_FRONT;
			//if (IsEqualIgnoreCase(cullFaceName, "back")) cullFace = GL_FRONT_AND_BACK;
		}
	}
}
