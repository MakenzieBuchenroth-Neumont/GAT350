#pragma once

namespace nc {
	class Editor {
	public:
		 void update();
		 void proccessGui(class Scene* scene);
	private:

		enum eType
		{
			TEXTURE,
			MODEL,
			MATERIAL,
			SHADER
		};

		eType type = eType::TEXTURE;

		bool m_active = true;
		class Object* m_selected = nullptr;
	};
}