 #pragma once

#include <string>
#include <unordered_map>

#include <glm/mat4x4.hpp>

namespace rym
{
	namespace api
	{
		typedef uint32_t GLenum;

		class ShaderProgram
		{
		public:
			ShaderProgram();
			virtual ~ShaderProgram();

			void LoadFromFile(const std::string_view& filePath);
			void LoadFromMemory(const std::string_view& stringSource);

			void Bind() const;
			void UnBind() const;

			void SetUniformInt(const std::string_view& name, int32_t value) const;
			void SetUniformIntArray(const std::string_view& name, int32_t* value, uint32_t count) const;
			void SetUniformFloat(const std::string_view& name, float value) const;
			void SetUniformVec2f(const std::string_view& name, const glm::vec2& v) const;
			void SetUniformVec3f(const std::string_view& name, const glm::vec3& v) const;
			void SetUniformVec4f(const std::string_view& name, const glm::vec4& v) const;
			void SetUniformMat3f(const std::string_view& name, const glm::mat3& m) const;
			void SetUniformMat4f(const std::string_view& name, const glm::mat4& m) const;

		private:
			uint32_t m_Program;
			std::string m_Name;

			void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);

			std::unordered_map<GLenum, std::string> SeparateShaders(const std::string_view& source);

			mutable std::unordered_map<std::string_view, int> m_UniformLocation;

			int GetUniformLocation(const std::string_view& name) const;
		};
	}
}