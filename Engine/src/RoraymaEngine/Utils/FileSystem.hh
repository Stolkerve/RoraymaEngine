#pragma once
#include <filesystem>

namespace rym {
	class FileSystem
	{
	public:
		static std::filesystem::path GetWorkSpacePath();

		static bool Move(const std::filesystem::path& from, const std::filesystem::path& to);

		static bool Rename(const std::filesystem::path& oldN, const std::filesystem::path& newN) noexcept;

		static bool Delete(const std::filesystem::path& file) noexcept;

		/// path of the parent folder + name of the new folder
		static bool CreateNewFolder(const std::filesystem::path& newFolder) noexcept;

		/// Path of the parent folder + name of the new file + extencion
		static bool CreateNewFile(const std::filesystem::path& newFile, const char* data = nullptr) noexcept;

		static bool MoveToReciclyBin(const std::filesystem::path& path) noexcept;
	};
}