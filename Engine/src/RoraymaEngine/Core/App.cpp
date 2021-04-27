#include "App.hh"

#include <imgui.h>
#include <examples/imgui_impl_opengl3.h>
#include <examples/imgui_impl_glfw.h>

#include <GLFW/glfw3.h>

#include <pybind11/embed.h>

#include "Input.hh"
namespace rym
{
	using namespace api;
	static auto workSpacePath = std::filesystem::current_path();
	App* App::s_Instance = nullptr;
	App::App()
	{
		s_Instance = this;

		m_Window = std::make_unique<Window>();

#if 0
		HWND hwnd = GetConsoleWindow();
		ShowWindow(hwnd, SW_SHOW);
#endif
		Input::Init();
		Renderer::Init();
		Renderer2D::Init();

		m_ImGuiLayer = new ImGuiLayer;
		PushOverlay(m_ImGuiLayer);
		pybind11::initialize_interpreter();
		{
			try {
				pybind11::module::import("sys").attr("path").attr("append")(pybind11::str(workSpacePath.string() + "\\assets\\scripts"));
			}
			catch (pybind11::error_already_set& e) {
				RYM_ASSERT(false, "Error to set the HOLY AND UNTOUCH scripts folder in the sys.path list");
				RYM_ERROR(e.what());
			}
		}
	}

	App::~App()
	{
		Renderer2D::Shutdown();
		pybind11::finalize_interpreter();
	}

	void App::Run()
	{
		Event event;
		while (m_Running)
		{
			while (m_Window->PollEvents(event))
			{
				for(const auto& s : m_LayerStack)
					s->OnEvent(event);

				if (event.type == Event::EventType::WINDOW_CLOSED_EVENT)
					m_Running = false;
				if (event.type == Event::EventType::WINDOW_MINIMIZE_EVENT)
					m_Minimized = true;
				else
					m_Minimized = false;
				//if (event.type == Event::EventType::WINDOW_RESIZED_EVENT)
				//	Renderer::windowResize(event.WindowSize.width, event.WindowSize.height);
			} // events loop

			m_CurrentTime = (float)glfwGetTime();
			m_DeltaTime = m_CurrentTime - m_LastTime;

			if (!m_Minimized)
			{
				Input::UpdateMouseDelta();

				for (const auto& s : m_LayerStack)
					s->OnUpdate(m_DeltaTime);

				m_ImGuiLayer->Begin();
				for (const auto& s : m_LayerStack)
					s->OnImGui();
				m_ImGuiLayer->End();

			} // minimized?
			//swaps buffers
			m_Window->Display();

			m_LastTime = m_CurrentTime;
		} // main loop

		for (const auto& s : m_LayerStack)
			s->OnQuit();
	}

	void App::PushLayer(Layer* Layer)
	{
		m_LayerStack.emplace(m_LayerStack.begin(),Layer);
		Layer->OnStart();
	}

	void App::PushOverlay(Layer* overlay)
	{
		m_LayerStack.emplace_back(overlay);
		overlay->OnStart();
	}

	void App::PopLayer(Layer* Layer)
	{

	}

	void App::ShutDown()
	{
		m_Running = false;
	}
}
