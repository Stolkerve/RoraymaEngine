#include "Input.hh"

#include "App.hh"
#include <GLFW/glfw3.h>

namespace rym
{
	Input::MouseData Input::s_MouseData;

	void Input::Init()
	{
		s_MouseData.scrollChange = false;
		s_MouseData.scroll = { 0.f, 0.f };
		s_MouseData.cursors[0] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
		s_MouseData.cursors[1] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
		s_MouseData.cursors[2] = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
		s_MouseData.cursors[3] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
		s_MouseData.cursors[4] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
		s_MouseData.cursors[5] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
	}

	bool Input::IsKeyPressed(KeysCode code)
	{
		int action = glfwGetKey(App::GetInstance().GetWindow()->GetContextWindow(), (int)code);
		return action == GLFW_PRESS;
	}

	bool Input::IsKeyJustPressed(KeysCode code)
	{
		int action = glfwGetKey(App::GetInstance().GetWindow()->GetContextWindow(), (int)code);
		static bool re = false;
		if (action && !re)
		{
			re = action;
			return action;
		}
		else
		{
			re = action;
			return false;
		}
	}

	bool Input::IsKeyRealese(KeysCode code)
	{
		int action = glfwGetKey(App::GetInstance().GetWindow()->GetContextWindow(), (int)code);
		return action == GLFW_RELEASE;
	}

	bool Input::IsButtonPressed(MouseCode button)
	{
		int action = glfwGetMouseButton(App::GetInstance().GetWindow()->GetContextWindow(), (int)button);
		return action;
	}

	bool Input::IsButtonRelease(MouseCode button)
	{
		int action = glfwGetMouseButton(App::GetInstance().GetWindow()->GetContextWindow(), (int)button);
		return !action;
	}

	bool Input::IsButtonJustPressed(MouseCode button)
	{
		int action = glfwGetMouseButton(App::GetInstance().GetWindow()->GetContextWindow(), (int)button);
		static bool re = false;
		if (action && !re)
		{
			re = action;
			return action;
		}
		else
		{
			re = action;
			return false;
		}
	}


	void Input::SetCursorShape(CursorShape shape)
	{
		//auto window = App::GetInstance().GetWindow()->GetContextWindow();

		auto a = s_MouseData.cursors[(int)shape];
		//auto b = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
		//auto c = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
		//glfwSetCursor(App::GetInstance().GetWindow()->GetContextWindow(), c);
		glfwSetCursor(App::GetInstance().GetWindow()->GetContextWindow(), (GLFWcursor*)a);
	}

	glm::vec2 Input::GetCursorPosition()
	{
		glm::dvec2 pos{0};
		glfwGetCursorPos(App::GetInstance().GetWindow()->GetContextWindow(), &pos.x, &pos.y);

		return glm::fvec2(pos);
	}

	glm::vec2 Input::GetCursorViewportPosition()
	{
		return s_MouseData.cursorPosition;
	}

	glm::vec2 Input::GetMouseScrolled()
	{
		if (s_MouseData.scrollChange)
		{
			s_MouseData.scrollChange = false;
			return s_MouseData.scroll;
		}
		else
		{
			return glm::vec2(0.f, 0.f);
		}
	}

	glm::vec2 Input::GetMouseDelta()
	{
		return s_MouseData.deltaMouse;
	}

	void Input::SetCursorMode(CursorMode mode)
	{
		glfwSetInputMode(App::GetInstance().GetWindow()->GetContextWindow(), GLFW_CURSOR, (int)mode);
	}

	void Input::SetScroll(float x, float y)
	{
		s_MouseData.scroll = { x, y };
		s_MouseData.scrollChange = true;
	}

	void Input::SetCursorPosition(float x, float y)
	{
		s_MouseData.cursorPosition = { x, y };
	}

	void Input::UpdateMouseDelta()
	{
		s_MouseData.firstMouse = GetCursorPosition();
		s_MouseData.deltaMouse = s_MouseData.firstMouse - s_MouseData.lastMouse;
		s_MouseData.lastMouse = s_MouseData.firstMouse;
	}
}
