#pragma once

#include <memory>

#include "../Renderer/Window.hh"
#include "Layer.hh"
#include "Clock.hh"

#include "../Renderer/Renderer.hh"
#include "../Renderer/Renderer2D.hh"

#include "ImGui/ImGuiLayer.hh"

// must be define in the client 
int main();

namespace rym
{
	class  App
	{
	public:
		App();
		App(const App&) = delete;
		App& operator=(const App&) = delete;
		virtual ~App();

		void Run();
		void PushLayer(Layer* Layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* newLayer);
		void ShutDown();

		static App& GetInstance() { return *s_Instance; }
		Window* GetWindow() { return m_Window.get(); }
		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
	private:
		// Windows
		std::unique_ptr<Window> m_Window;

		bool m_Running = true;
		//Layer
		std::vector<Layer*> m_LayerStack;

		ImGuiLayer* m_ImGuiLayer;

		// Delta time
		float m_CurrentTime = 0.f, m_DeltaTime = 0.f, m_LastTime = 0.f;
		//
		bool m_Minimized = false;

		static App* s_Instance;
	};

	// must be define in the client 
	App* CreateApp();
}
