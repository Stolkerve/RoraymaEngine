#pragma once

#include <glm/vec4.hpp>

namespace rym
{
	class Color
	{
	public:
		Color() = default;

		Color(const glm::vec4& color)
		{
			rgba = color;
		}

		Color(const Color& color)
		{
			rgba = color.rgba;
		}

		Color(float r, float g, float b, float a = 255.f)
		{
			rgba = { r, g, b, a };
		}

		void setColor(const glm::vec4& color) { rgba = color; }
		const glm::vec4& GetColor() const { return rgba; }

		static const glm::vec4 WHITE;
		static const glm::vec4 BLACK;
		static const glm::vec4 RED;
		static const glm::vec4 GREEN;
		static const glm::vec4 BLUE;
		static const glm::vec4 YELLOW;
		static const glm::vec4 CYAN;
		static const glm::vec4 MAGENTA;

		glm::vec4 rgba = {0.f, 0.f, 0.f, 255.f};
	};

}
