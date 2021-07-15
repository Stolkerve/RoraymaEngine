#pragma once

#include <RoraymaEngine/RoraymaEngine.hh>
#include <filesystem>

namespace rym
{
	struct FoldersData
	{
		FoldersData() = default;
		FoldersData(const std::filesystem::path& path, bool open) :
			Path(path), Open(open)
		{}
		std::filesystem::path Path = "";
		std::filesystem::path Extension = "";
		bool Open = false;
		bool NeedUpdate = false;
		bool NeedDelete = false;
		bool isDirectory = false;
		bool isRoot = false;
	};

	using NodeFolder = Node<FoldersData>;

	enum class Extencions {
		unknow,
		code,
		image,
		git,
		music,
		rym,
		text
	};

	class FoldersPanel
	{
	public:
		FoldersPanel();
		void Render();
		void SetChangeSceneCallBack(std::function<void(const std::string&)> callback) { m_ChangeSceneFunc = callback; }
	private:
		void Refresh();
		void DrawNode(std::shared_ptr<NodeFolder>& node, int id);
		void IterateNodeFolder(const std::shared_ptr<NodeFolder>& node);
		std::string IconFilter(const std::filesystem::path& path);
		NodeFolder* m_HoveredItem = nullptr;
		NodeFolder* m_SelectedItem;
		struct DeletedItem
		{
			std::filesystem::path FilePath;
			std::filesystem::path ParentPath;
			//bool NeedDelete = false;
			//bool NeedRootDelete = false;
		}m_DeletedItem;
		std::function<void(const std::string&)> m_ChangeSceneFunc;
		std::shared_ptr<NodeFolder> m_Root;
		std::shared_ptr<Scene> m_CurrentScene;
		std::map<std::string_view, Extencions> m_Extencions;
		float timer = 0.f;
	};
};
