#pragma once

#include <glm/vec2.hpp>

#include "InputCodes.hh"

namespace rym
{
	enum class CursorShape
	{
		ARROW,
		IBEAM,
		CROSSHAIR,
		HAND,
		HRESIZE,
		VRESIZE
	};

	enum class CursorMode
	{
		NORMAL = 0x00034001,
		HIDDEN = 0x00034002,
		DISABLED = 0x00034003
	};

	class Input
	{
	public:
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;

		static void Init();

		/*************/
		/* Keyboard */
		/***********/

		// Return true if the key is repeat
		static bool IsKeyPressed(KeysCode code);
		static bool IsKeyJustPressed(KeysCode code);
		static bool IsKeyRealese(KeysCode code);

		/**********/
		/* Mouse */
		/********/
		static bool IsButtonPressed(MouseCode button);
		static bool IsButtonJustPressed(MouseCode button);
		static bool IsButtonRelease(MouseCode button);

		// Return the position of the cursor on the context window.
		static glm::vec2 GetCursorPosition();
		static glm::vec2 GetCursorWorldPosition();
		static glm::vec2 GetMouseScrolled();
		static glm::vec2 GetMouseDelta();
		static glm::vec2 GetMouseWorldDelta();

		static void SetMousePosition(const glm::vec2& v);
		static void SetMouseJustPressed(bool action);

		static void SetCursorShape(CursorShape shape);
		static void SetCursorMode(CursorMode mode);

		static void SetScroll(float x, float y);

		static void SetCursorWorldPosition(const glm::vec2& v);
		static void SetMouseWorldDelta(const glm::vec2& v );

		static void UpdateMouseDelta();
		// vsg latam
	private:
		struct MouseData
		{
			bool scrollChange;
			bool mouseOnce;

			glm::vec2 scroll;

			glm::vec2 firstMouse;
			glm::vec2 lastMouse;
			glm::vec2 mouseDelta;

			glm::vec2 cursorWorldPosition;
			glm::vec2 mouseWorldDelta;

			void* cursors[6];
		};

		static MouseData s_MouseData;
	};
}
