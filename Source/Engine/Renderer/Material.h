#pragma once
#include "Framework/Resource/Resource.h"
#include "GUI.h"
#include <glm/glm/glm.hpp>
#include <vector>
#include <memory>

namespace nc
{
	class Texture;
	class Program;

	class Material : public Resource
	{
	public:
		const uint32_t ALBEDO_TEXTURE_MASK	 = (1 << 0); // 0001
		const uint32_t SPECULAR_TEXTURE_MASK = (1 << 1); // 0010
		const uint32_t NORMAL_TEXTURE_MASK	 = (1 << 2); // 0100
		const uint32_t EMISSIVE_TEXTURE_MASK = (1 << 3); // 1000

	public:
		virtual bool Create(std::string filename, ...) override;

		void Bind();

		res_t<Program> GetProgram() { return m_program; }
		void processGui();

	public:
		glm::vec3 albedo{ 1 };
		glm::vec3 specular{ 1 };
		glm::vec3 emissive{ 0 };
		float shininess = 2.0f;

		glm::vec2 tiling{ 1, 1 };
		glm::vec2 offset{ 0, 0 };

		res_t<Texture> albedoTexture;
		res_t<Texture> specularTexture;
		res_t<Texture> normalTexture;
		res_t<Texture> emissiveTexture;

	private:
		res_t<Program> m_program;
		//std::vector<res_t<Texture>> m_textures;
	};
}