#pragma once

#include"Scene.hh"

namespace rym
{
	class SceneSerializer
	{
	public:
		static void SceneSerialize(const std::shared_ptr<Scene>& scene, const std::string& path);

		static bool SceneDesSerialize(const std::shared_ptr<Scene>& scene, const std::string& path);
	};
}