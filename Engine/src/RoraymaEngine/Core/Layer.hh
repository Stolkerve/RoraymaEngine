#pragma once
#include "../events/Event.hh"
#include <string>

namespace rym
{
	enum class LayerMode
	{
		EDITOR_MODE,
		GAME_MODE
	};

	class  Layer
	{
	public:
		Layer(const std::string_view& name = "Layer");
		virtual ~Layer() {};

		virtual void OnStart() {};
		virtual void OnQuit() {};
		virtual void OnEvent(const Event& events) {};
		virtual void OnUpdate(float _delta) {};
		virtual void OnImGui() {};

		inline std::string GetName() const { return m_Name; }
	protected:
		std::string m_Name;
	};
}