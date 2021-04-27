#pragma once

#include<string>
#include <queue>

#include "../events/Event.hh"
#include <glm/vec2.hpp>

typedef struct GLFWmonitor GLFWmonitor;
typedef struct GLFWwindow GLFWwindow;

namespace rym
{
	namespace api
	{
		struct Size
		{
			uint32_t widht, height;
		};

		class Window
		{
		public:
			Window();
			virtual ~Window();

			bool PollEvents(Event& e);
			void Display();

			void SetClose(bool value);
			bool Closed();

			void setWindowConfig();

			GLFWwindow* GetContextWindow() { return m_Window; }

			const Size& GetWindowSize() const { return m_WindowData.size; }

		private:
			void InitGLFW();
			void InitGLAD();

			bool PopEvent(Event& event, bool block);
			void PushEvent(Event& event);

		private:
			GLFWwindow* m_Window;
			GLFWmonitor* m_PrimaryMonitor;

			struct WindowData
			{
				Size size;
				std::string Title;

				bool Fullscreen;
				bool V_Sync;
				bool Resize;

				Event e;
				std::queue<Event> Events;
			};

			WindowData m_WindowData;

		};
	}
}
