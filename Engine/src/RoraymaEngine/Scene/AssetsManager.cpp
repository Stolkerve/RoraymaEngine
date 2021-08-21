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
		return GetTexture(name);
		//RYM_CORE_ERROR(false, "ERROR: Cannot exists two o more textures with the same name and the extension is ignored. \nTexture: {0}\nPath: {1}", name, path);
	}

	void AssetsManager::PushTexture(const std::string& name, uint32_t width, uint32_t height, uint32_t* data)
	{
		auto it = m_Data.texturesConteiner.find(name);
		if (it == m_Data.texturesConteiner.end())
		{
			m_Data.texturesConteiner.insert({ name, std::make_shared<api::Texture2D>(width, height, data) });
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

/*	void AssetsManager::PushFont(const std::string& name, const std::string& path)
	{
		auto it = m_Data.charactersConteiner.find(name);
		if (it == m_Data.charactersConteiner.end())
		{
			m_Data.charactersConteiner.insert({ name, std::make_shared<Characters>(path) });
			return;
		}
	}*/

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

	//std::shared_ptr<Characters> AssetsManager::GetFont(const std::string& name)
	//{
	//	auto it = m_Data.charactersConteiner.find(name);
	//	if (it == m_Data.charactersConteiner.end())
	//	{
	//		//RYM_CORE_ASSERT(false, "ERROR: Could not find the texture");
	//		return nullptr;
	//	}

	//	return m_Data.charactersConteiner.at(name);
	//}

	void AssetsManager::DeleteTexture(const std::string& name)
	{
	}
}
