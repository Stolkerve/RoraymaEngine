#include "AssetsManager.hh"
#include <RoraymaEngine/Core/Assert.hh>

namespace rym
{
	AssetsManager::Data AssetsManager::m_Data;

	std::shared_ptr<api::Texture2D> AssetsManager::PushTexture(const std::string& name, const std::string& path)
	{
		auto it = m_Data.texturesConteiner.find(name);
		if (it == m_Data.texturesConteiner.end())
		{
			auto newTex = std::make_shared<api::Texture2D>(path);
			m_Data.texturesConteiner.insert({ name, newTex });
			return newTex;
		}

		//RYM_CORE_ERROR(false, "ERROR: Cannot exists two o more textures with the same name and the extension is ignored. \nTexture: {0}\nPath: {1}", name, path);
	}

	void AssetsManager::PushTexture(const std::string& name, uint32_t width, uint32_t height, uint32_t* data)
	{
		auto it = m_Data.texturesConteiner.find(name);
		if (it == m_Data.texturesConteiner.end())
		{
			m_Data.texturesConteiner.insert({ name, std::make_shared<api::Texture2D>(width, height) });
			m_Data.texturesConteiner.at(name)->SetData(data);
			return;
		}
	}

	void AssetsManager::PushShader(const std::string& name, const std::string& path)
	{
		auto it = m_Data.shadersConteiner.find(name);
		if (it == m_Data.shadersConteiner.end())
		{
			m_Data.shadersConteiner.insert({ name, std::make_shared<api::ShaderProgram>() });
			m_Data.shadersConteiner.at(name)->LoadFromFile(path);
			return;
		}
	}

	std::shared_ptr<api::Texture2D> AssetsManager::GetTexture(const std::string& name)
	{
		auto it = m_Data.texturesConteiner.find(name);
		if (it == m_Data.texturesConteiner.end())
		{
			//RYM_CORE_ASSERT(false, "ERROR: Could not find the texture");
			return nullptr;
		}

		return m_Data.texturesConteiner.at(name);
	}

	std::shared_ptr<api::ShaderProgram> AssetsManager::GetShader(const std::string& name)
	{
		auto it = m_Data.shadersConteiner.find(name);
		if (it == m_Data.shadersConteiner.end())
		{
			//RYM_CORE_ASSERT(false, "ERROR: Could not find the texture");
			return nullptr;
		}

		return m_Data.shadersConteiner.at(name);
	}

	void AssetsManager::DeleteTexture(const std::string& name)
	{
	}
}
