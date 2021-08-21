#include "ShaderProgram.hh"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "../Utils/ReadTextFiles.hh"
#include "../Core/Assert.hh"

namespace rym
{
	namespace api
	{
		static const uint32_t SHADER_PROGRAM = 0;

		ShaderProgram::ShaderProgram()
		{}

		void ShaderProgram::LoadFromFile(const std::string_view& filePath)
		{
			std::string source = utils::read_file(filePath);
			auto shaderSources = SeparateShaders(source);
			Compile(shaderSources);

			// Extract name from filepath
			std::filesystem::path path = filePath;
			m_Name = path.stem().string();
		}

		void ShaderProgram::LoadFromMemory(const std::string_view& stringSource)
		{
			auto shaderSources = SeparateShaders(stringSource);
			Compile(shaderSources);

			m_Name = stringSource;
		}

		void ShaderProgram::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
		{
			GLuint program = glCreateProgram();
			RYM_CORE_ASSERT(shaderSources.size() <= 2, "We only support 2 shaders for now");
			std::array<GLenum, 2> glShaderIDs;
			int glShaderIDIndex = 0;
			for (auto& kv : shaderSources)
			{
				GLenum type = kv.first;
				const std::string& source = kv.second;

				GLuint shader = glCreateShader(type);

				const GLchar* sourceCStr = source.c_str();
				glShaderSource(shader, 1, &sourceCStr, 0);

				glCompileShader(shader);

				GLint isCompiled = 0;
				glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
				if (isCompiled == GL_FALSE)
				{
					GLint maxLength = 0;
					glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

					std::vector<GLchar> infoLog(maxLength);
					glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

					glDeleteShader(shader);

					RYM_CORE_ERROR("{0} {1}", m_Name, infoLog.data());
					RYM_CORE_ASSERT(false, "Shader compilation failure!");
					break;
				}

				glAttachShader(program, shader);
				glShaderIDs[glShaderIDIndex++] = shader;
			}

			m_Program = program;

			// Link our program
			glLinkProgram(program);

			// Note the different functions here: glGetProgram* instead of glGetShader*.
			GLint isLinked = 0;
			glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
			if (isLinked == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

				// We don't need the program anymore.
				glDeleteProgram(program);

				for (auto id : glShaderIDs)
					glDeleteShader(id);

				RYM_CORE_ERROR("{0}", infoLog.data());
				RYM_CORE_ASSERT(false, "Shader link failure!");
				return;
			}

			for (auto id : glShaderIDs)
			{
				glDetachShader(program, id);
				glDeleteShader(id);
			}
		}

		std::unordered_map<GLenum, std::string> ShaderProgram::SeparateShaders(const std::string_view& source)
		{
			std::unordered_map<GLenum, std::string> shaderSources;

			auto ShaderTypeFromString = [](const std::string_view& type)
			{
				if (type == "vertex")
					return GL_VERTEX_SHADER;
				if (type == "fragment" || type == "pixel")
					return GL_FRAGMENT_SHADER;
				else
					RYM_CORE_ASSERT(false, "Unknown shader type!");
			};

			const char* typeToken = "#type";
			size_t typeTokenLength = strlen(typeToken);
			size_t pos = source.find(typeToken, 0); //Start of shader type declaration line
			while (pos != std::string::npos)
			{
				size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
				RYM_CORE_ASSERT(eol != std::string::npos, "Syntax error");
				size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
				std::string&& type = std::string(source.data()).substr(begin, eol - begin);
				RYM_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

				size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
				RYM_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
				pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line

				shaderSources[ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
			}

			return shaderSources;
		}

		void ShaderProgram::Bind() const
		{
			glUseProgram(m_Program);
		}

		void ShaderProgram::UnBind() const
		{
			glUseProgram(0);
		}

		void ShaderProgram::SetUniformInt(const std::string_view& name, int32_t value) const
		{
			glUniform1i(GetUniformLocation(name), value);
		}

		void ShaderProgram::SetUniformIntArray(const std::string_view& name, int32_t* value, uint32_t count) const
		{
			glUniform1iv(GetUniformLocation(name), count, value);
		}

		void ShaderProgram::SetUniformFloat(const std::string_view& name, float value) const
		{
			glUniform1f(GetUniformLocation(name), value);
		}

		void ShaderProgram::SetUniformVec2f(const std::string_view& name, const glm::vec2& v) const
		{
			glUniform2fv(GetUniformLocation(name), 1, glm::value_ptr(v));
		}

		void ShaderProgram::SetUniformVec3f(const std::string_view& name, const glm::vec3& v) const
		{
			glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(v));
		}

		void ShaderProgram::SetUniformVec4f(const std::string_view& name, const glm::vec4& v) const
		{
			glUniform4fv(GetUniformLocation(name), 1, glm::value_ptr(v));
		}

		void ShaderProgram::SetUniformMat3f(const std::string_view& name, const glm::mat3& m) const
		{
			glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(m));
		}

		void ShaderProgram::SetUniformMat4f(const std::string_view& name, const glm::mat4& m) const
		{
			glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(m));
		}

		int ShaderProgram::GetUniformLocation(const std::string_view& name) const
		{
			if (m_UniformLocation.find(name) != m_UniformLocation.end())
				return m_UniformLocation[name];

			m_UniformLocation[name] = glGetUniformLocation(m_Program, name.data());
			return m_UniformLocation[name];
		}

		ShaderProgram::~ShaderProgram()
		{
			glDeleteProgram(m_Program);
			//glDeleteShader(m_Program);
		}
	}
}
