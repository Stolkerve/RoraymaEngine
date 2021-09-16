#pragma once

#include <unordered_map>
#include <memory>

struct ImFont;

#include "../Renderer/Texture.hh"
#include "../Renderer/ShaderProgram.hh"
#include "../Renderer/Characters.hh"

namespace rym
{
	class AssetsManager
	{
	public:
		static std::shared_ptr<api::Texture2D> PushTexture(const std::string& name, const std::string& path);
		static void PushTextureFromMemory(const std::string& name, uint32_t width, uint32_t height, uint32_t* data);
		static void PushTextureFromMemoryBase64(const std::string& name, const std::string& source);

		static void PushShaderFromMemory(const std::string& name, const std::string& source);
		static void PushShader(const std::string& name, const std::string& path);

		static void PushFont(const std::string& name, const std::string& path, float fontSize);
		static void PushFontFromMemoryBase64(const std::string& name, const std::string& source, float fontSize);

		static void PushImFont(const std::string& name, ImFont* font);

		static std::shared_ptr<api::Texture2D> GetTexture(const std::string& name);
		static std::shared_ptr<api::ShaderProgram> GetShader(const std::string& name);
		static std::shared_ptr<Characters> GetFont(const std::string& name);
		static ImFont* GetImFont(const std::string& name);

		static void DeleteTexture(const std::string& name);
	private:
		static struct Data
		{
			std::unordered_map<std::string /* Name */, std::shared_ptr<api::Texture2D>> texturesConteiner;
			std::unordered_map<std::string /* Name */, std::shared_ptr<api::ShaderProgram>> shadersConteiner;
			std::unordered_map<std::string /* Name */, std::shared_ptr<Characters>> charactersConteiner;
			std::unordered_map<std::string /* Name */, ImFont*> imGuiFontsConteiner;
		}m_Data;
	};

}
