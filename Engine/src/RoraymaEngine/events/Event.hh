#pragma once

namespace rym
{
	class Event
	{
	public:
		Event() = default;
		~Event() = default;

    public:
        enum EventType
        {
            None = 0,
            WINDOW_CLOSED_EVENT, WINDOW_RESIZED_EVENT, WINDOW_MINIMIZE_EVENT, WINDOW_MAXIMIZED_EVENT, WINDOW_FOCUS_EVENT, WINDOW_LOST_FOCUS_EVENT, WINDOW_MOVED_EVENT,
            KEY_PRESSED_EVENT, KEY_REALEASED_EVENT, KEY_REPEATED_EVENT,
            MOUSE_MOVED_EVENT, MOUSE_BUTTON_PRESSED_EVENT, MOUSE_BUTTON_REALEASED_EVENT, MOUSE_SCROLLED_EVENT, MOUSE_ENTERED_EVENT
        };
        EventType type;

    private:
        struct MouseMoveEvent{
            float x;
            float y;
        };
        struct MouseScrollEvent{
             float x;
             float y;
        };
        struct MouseButtonEvent {
            int button;
            int mods;
        };
        struct WindowSizeEvent {
            int width;
            int height;
        };
        struct KeyEvent{
            int key;
            int scancode;
            int mods;
        };

    public:
        union
        {
            MouseMoveEvent MousePos;
            MouseScrollEvent MouseScroll;
            MouseButtonEvent MouseButton;
            WindowSizeEvent WindowSize;
            KeyEvent KeyBoard;
        };
	};
}