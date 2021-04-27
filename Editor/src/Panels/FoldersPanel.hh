#pragma once

#include <RoraymaEngine/RoraymaEngine.hh>
#include <filesystem>
#include <iterator>
#include <functional>
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

	// TODO: Its shouldn't exits here, its must be 
	class FilesHandler
	{
	public:
		static bool Move(const std::filesystem::path& from, const std::filesystem::path& to);
		static bool Rename(const std::filesystem::path& oldN, const std::filesystem::path& newN) noexcept;
		static bool Delete(const std::filesystem::path& file) noexcept;
		/// path of the parent folder + name of the new folder
		static bool CreateNewFolder(const std::filesystem::path& newFolder) noexcept;
		/// Path of the parent folder + name of the new file + extencion
		static bool CreateNewFile(const std::filesystem::path& newFile, const char* data = nullptr) noexcept;
		static bool MoveToReciclyBin(const std::filesystem::path& path) noexcept;
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
	};
};
