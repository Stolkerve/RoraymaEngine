#include "Window.hh"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <yaml-cpp/yaml.h>

#include "../Core/Assert.hh"

#define RYM_CORE_INPUTS_SETTERS
#include "../Core/Input.hh"

static std::string basic_window_config = R"(
fullscreen: false
height: 540
name: RoraymaEngine
resize: false
v_sync: true
width: 960
)";

namespace rym
{
	namespace api
	{
		Window::Window()
		{
			setWindowConfig();
			InitGLFW();
		}

		void Window::setWindowConfig()
		{
			YAML::Node node;
			try
			{
				node = YAML::LoadFile("./window_config.yaml");
				RYM_CORE_INFO("window_config loaded successfully");
			}
			catch (const YAML::ParserException& ex)
			{
				RYM_CORE_INFO("window_config dont load");
			}

			//node = YAML::Load(basic_window_config);
			//RYM_CORE_ASSERT(false,);
			m_WindowData.Fullscreen = node["fullscreen"].as<bool>();
			m_WindowData.Resize = node["resize"].as<bool>();
			m_WindowData.V_Sync = node["v_sync"].as<bool>();
			m_WindowData.Title = node["name"].as<std::string>();
			m_WindowData.size = { node["width"].as<uint32_t>(), node["height"].as<uint32_t>() };
		}

		bool Window::PollEvents(Event& e)
		{
			glfwPollEvents();
			if (PopEvent(e, false))
				return true;
			else
				return false;
		}

		bool Window::PopEvent(Event& event, bool block)
		{
			if (!m_WindowData.Events.empty())
			{
				event = m_WindowData.Events.front();
				m_WindowData.Events.pop();

				return true;
			}
			return false;
		}

		void Window::PushEvent(Event& event)
		{
			m_WindowData.Events.push(event);
		}

		void Window::InitGLAD()
		{
			if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) != 1)
			{
				glfwDestroyWindow(m_Window);
				glfwTerminate();
				RYM_CORE_ERROR("Could not init glad");
			}
		}

		void Window::InitGLFW()
		{
			if (!glfwInit())
			{
				RYM_CORE_ERROR("Could not init glfw");
			}
			glfwSetErrorCallback([](int error, const char* description)
				{
					RYM_CORE_ERROR(description);
				});

			m_PrimaryMonitor = glfwGetPrimaryMonitor();
			if (!m_PrimaryMonitor)
			{
				RYM_CORE_ERROR("No monitor have found");
			}

			//glfwWindowHint(GLFW_SAMPLES, 4);
			glfwWindowHint(GLFW_RESIZABLE, m_WindowData.Resize);
			glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
			/* Create a GLFW window */

			if (m_WindowData.Fullscreen)
			{
				m_Window = glfwCreateWindow(glfwGetVideoMode(glfwGetPrimaryMonitor())->width,
					glfwGetVideoMode(glfwGetPrimaryMonitor())->height, "My Title",
					glfwGetPrimaryMonitor(), nullptr);
			}
			else
				m_Window = glfwCreateWindow(m_WindowData.size.widht, m_WindowData.size.height, m_WindowData.Title.c_str(), NULL, NULL);
			if (!m_Window)
			{
				RYM_CORE_ERROR("Could not create a glfw window");
			}

			glfwMakeContextCurrent(m_Window);
			InitGLAD();

			// v-sync
			if (m_WindowData.V_Sync)
				glfwSwapInterval(1);
			else
				glfwSwapInterval(0);

			glfwSetWindowUserPointer(m_Window, &m_WindowData);

			/* set GLFW callbacks */

			// Inputs callbacks //
			glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

					switch (action)
					{
					case GLFW_PRESS:
					{
						data.e.type = Event::EventType::KEY_PRESSED_EVENT;
						data.e.KeyBoard = { key, scancode, mods };
						data.Events.push(data.e);
						break;
					}
					case GLFW_RELEASE:
					{

						data.e.type = Event::EventType::KEY_REALEASED_EVENT;
						data.e.KeyBoard = { key, scancode, mods };
						data.Events.push(data.e);
						break;
					}
					case GLFW_REPEAT:
					{

						data.e.type = Event::EventType::KEY_REPEATED_EVENT;
						data.e.KeyBoard = { key, scancode, mods };
						data.Events.push(data.e);
						break;
					}
					}
				});
			glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
					if (button == GLFW_MOUSE_BUTTON_1 && action != GLFW_RELEASE)
					{
						Input::SetMouseJustPressed(true);
					}
					else
						Input::SetMouseJustPressed(false);
					switch (action)
					{
					case GLFW_PRESS:
					{
						data.e.type = Event::EventType::MOUSE_BUTTON_PRESSED_EVENT;
						data.e.MouseButton = { button, mods };
						data.Events.push(data.e);
						break;
					}
					case GLFW_RELEASE:
					{
						//Input::SetMouseJustPressed(true);

						data.e.type = Event::EventType::MOUSE_BUTTON_REALEASED_EVENT;
						data.e.MouseButton = { button, mods };
						data.Events.push(data.e);
						break;
					}
					}
				});
			glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
					data.e.type = Event::EventType::MOUSE_MOVED_EVENT;
					data.e.MousePos = { (float)xPos, (float)yPos };
					data.Events.push(data.e);
				});
			glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

					Input::SetScroll((float)xOffset, (float)yOffset);

					data.e.type = Event::EventType::MOUSE_SCROLLED_EVENT;
					data.e.MouseScroll = { (float)xOffset, (float)yOffset };
					data.Events.push(data.e);
				});
			glfwSetCursorEnterCallback(m_Window, [](GLFWwindow* window, int entered)
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

					data.e.type = Event::EventType::MOUSE_ENTERED_EVENT;
					data.Events.push(data.e);
				});
			glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

					// this is use inside window class
					data.size = { uint32_t(width), uint32_t(height) };

					data.e.type = Event::EventType::WINDOW_RESIZED_EVENT;
					// this is use insede event class
					data.e.WindowSize = { width, height };

					if (width == 0 || height == 0)
						data.e.type = Event::EventType::WINDOW_MINIMIZE_EVENT;

					data.Events.push(data.e);
				});
			glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

					data.e.type = Event::EventType::WINDOW_CLOSED_EVENT;
					data.Events.push(data.e);
				});
			glfwSetWindowMaximizeCallback(m_Window, [](GLFWwindow* window, int maximized)
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
					if (maximized)
					{
						data.e.type = Event::EventType::WINDOW_MAXIMIZED_EVENT;
						data.Events.push(data.e);
					}
				});
			glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int focused)
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
					if (focused)
					{
						data.e.type = Event::EventType::WINDOW_FOCUS_EVENT;
						data.Events.push(data.e);
					}
					else
					{
						data.e.type = Event::EventType::WINDOW_LOST_FOCUS_EVENT;
						data.Events.push(data.e);
					}
				});
		}

		Window::~Window()
		{
			glfwDestroyWindow(m_Window);
			glfwTerminate();
		}

		bool Window::Closed()
		{
			return glfwWindowShouldClose(m_Window);
		}

		void Window::Display()
		{
			glfwSwapBuffers(m_Window);
		}

		void Window::SetClose(bool value)
		{
			glfwSetWindowShouldClose(m_Window, value);
		}
	}
}
