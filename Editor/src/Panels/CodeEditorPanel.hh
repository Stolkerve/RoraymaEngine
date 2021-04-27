#pragma once

#include <ImGuiColorTextEdit/TextEditor.h>
#include <filesystem>

namespace rym
{
	struct CodeFile
	{
		std::string data;
		std::filesystem::path path;
		std::filesystem::path extension;
		std::filesystem::path name;
		bool selected = false;
		bool change = false;
		bool open = false;
	};

	class CodeEditorPanel
	{
	public:
		static void Init();
		static void Render();
		static void PushFile(const std::filesystem::path& path);
	private:
		CodeEditorPanel() = default;
		~CodeEditorPanel() = default;
		static void OverWriteFile(CodeFile& codeFile);
		static void ReadFile(CodeFile& codeFile);

		static TextEditor m_TextEditor;
		static std::vector<CodeFile> m_CodeFiles;
	};
}
