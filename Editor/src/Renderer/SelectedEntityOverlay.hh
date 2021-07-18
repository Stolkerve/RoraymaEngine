#pragma once
#include <RoraymaEngine/Renderer/Camera.hh>
#include <RoraymaEngine/Scene/Entity.hh>

namespace rym
{
	// SEO stands for Selected Entity Object
	class SEOverlay
	{
	public:
		static void DrawOverlay(const std::shared_ptr<Entity>& entity, const EditorCamera& editorCamera, bool& blockSelectedEntity);
	};
}
