#pragma once
#include <RoraymaEngine/Renderer/Camera.hh>
#include <RoraymaEngine/Scene/Entity.hh>

namespace rym
{
	// SEO stands for Selected Entity Object
	class SEOverlay
	{
	public:
		void DrawOverlay(const std::shared_ptr<Entity>& entity, const EditorCamera& editorCamera, bool& blockSelectedEntity);

	private:
		bool m_SpriteCir1 = false;
		bool m_SpriteCir2 = false;
		bool m_SpriteCir3 = false;
		bool m_SpriteCir4 = false;
	};
}
