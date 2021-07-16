#include "FileSystem.hh"
#include "../Core/Log.hh"

#ifdef RYM_PLATFORM_WINDOWS
	#include <shellapi.h>
	#include <Windows.h>
#endif

#include <fstream>

std::filesystem::path rym::FileSystem::GetWorkSpacePath()
{
    return std::filesystem::current_path();
}

bool rym::FileSystem::Move(const std::filesystem::path& from, const std::filesystem::path& to)
{
	std::error_code ec;
	if (!std::filesystem::is_directory(from))
	{
		auto tmpTo = to / from.filename();
		std::filesystem::rename(from, tmpTo, ec);
	}
	else
	{
		std::filesystem::rename(from, to, ec);
	}

	if (ec.value())
	{
		RYM_CRITICAL("{0} was not moved to {1}. Error messege: {2} \n Error Code: {3}",
			from.filename().string(), to.string(), ec.message(), ec.value());
		return false;
	}
	RYM_WARN("{0} was moved to {1} successfully", from.filename().string(), to.string());
	return true;
}

bool rym::FileSystem::Rename(const std::filesystem::path& oldN, const std::filesystem::path& newN) noexcept
{
	auto newName = (oldN.parent_path() / newN.filename());
	int success = std::rename(oldN.string().c_str(), newName.string().c_str());
	if (!success)
	{
		RYM_CRITICAL("{} was not renamed", oldN.filename().string());
		return false;
	}
	RYM_WARN("{} was renamed successfully", oldN.filename().string());
	return true;
}

bool rym::FileSystem::Delete(const std::filesystem::path& file) noexcept
{
	int success = std::remove(file.string().c_str());
	if (!success)
	{
		RYM_CRITICAL("{} was not deleted.", file.filename().string());
		return false;
	}
	RYM_WARN("{} was deleted successfully", file.filename().string());
	return true;
}

bool rym::FileSystem::CreateNewFolder(const std::filesystem::path& newFolder) noexcept
{
	std::error_code ec;
	if (std::filesystem::create_directory(newFolder, ec))
	{
		RYM_WARN("{} folder was created succesfully", newFolder.filename().string());
		return true;
	}
	RYM_CRITICAL("{0} folder was not created. Error: {1}", newFolder.filename().string(), ec.message());
	return false;
}

bool rym::FileSystem::CreateNewFile(const std::filesystem::path& newFile, const char* data) noexcept
{
	std::ofstream file(newFile);
	if (file)
	{
		file << data;
		file.close();
		RYM_WARN("{} file was created succesfully", newFile.filename().string());
		return true;
	}
	RYM_CRITICAL("{} file was not created", newFile.filename().string());
	return false;
}

bool rym::FileSystem::MoveToReciclyBin(const std::filesystem::path& path) noexcept
{
#ifdef RYM_PLATFORM_WINDOWS
	SHFILEOPSTRUCT fileOp;
	ZeroMemory(&fileOp, sizeof(SHFILEOPSTRUCT));
	fileOp.hwnd = NULL;
	fileOp.wFunc = FO_DELETE;
	//auto temp = path.string() + "\0\0";
	auto temp2 = path.wstring();
	temp2.append(1, '\0');
	fileOp.pFrom = temp2.c_str();
	fileOp.pTo = NULL;
	fileOp.fFlags = FOF_ALLOWUNDO | FOF_NOERRORUI | FOF_NOCONFIRMATION | FOF_SILENT;
	int res = SHFileOperation(&fileOp);
	if (res == 0 && !fileOp.fAnyOperationsAborted)
	{
		RYM_WARN("{} was moved to trash successfully", path.filename().string());
		return true;
	}
	RYM_CRITICAL("{0} was not moved to trash. Error {1}. Look SHFileOperation errors", path.filename().string(), res);
	return false;
#else
	std::string HOME_DIR(std::getenv("HOME"));
	if (HOME_DIR.empty())
	{
		RYM_CRITICAL("No home path was fount, linux only error");
		return false;
	}
	else
	{
		HOME_DIR += "/.local/share/Trash/files";
		Move(path, HOME_DIR);
	}
#endif
}
