#pragma once
#include "RenderComponent.h"
#include "Renderer/Model.h"

namespace nc
{
	class ModelComponent : public RenderComponent
	{
	public:
		CLASS_DECLARATION(ModelComponent)

		bool Initialize() override;
		void Update(float dt) override;
		void Draw(class Renderer& renderer) override;

		void processGui() override;

	public:
		std::string modelName;
		std::string materialName;

		res_t<Model> model;
		res_t<Material> material;

		bool castShadow{ true };
		bool enableDepth{ true };
		GLint cullFace{ GL_BACK };
	};
}