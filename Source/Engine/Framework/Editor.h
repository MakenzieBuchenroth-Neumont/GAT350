#pragma once

namespace nc {
	class Editor {
	public:
		 void proccessGui(class Scene* scene);
	private:
		class Object* m_selected = nullptr;
	};
}