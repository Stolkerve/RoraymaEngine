#include "CodeEditorPanel.hh"
#include <RoraymaEngine/Core/Log.hh>
#include <RoraymaEngine/Core/Input.hh>
#include <fstream>
#include <sstream>
#include "FoldersPanel.hh"

namespace rym
{
	TextEditor CodeEditorPanel::m_TextEditor;
	std::vector<CodeFile> CodeEditorPanel::m_CodeFiles;

	void CodeEditorPanel::Init()
	{
		/*Desserialize*/

		m_TextEditor.SetPalette(m_TextEditor.GetDarkPalette());
		m_TextEditor.SetLanguageDefinition(TextEditor::LanguageDefinition::Python());
		m_TextEditor.SetShowWhitespaces(false);
		m_TextEditor.SetShowTaps(true);

		auto scriptsFolder = std::filesystem::current_path().string() + "//assets//scripts";
		size_t i = 0;
		for (const auto& entry : std::filesystem::directory_iterator(scriptsFolder,
			std::filesystem::directory_options::skip_permission_denied))
		{
			auto& filePath = entry.path();
			if (filePath.extension() == ".py")
			{
				CodeFile codeFile;
				codeFile.path = filePath;
				codeFile.extension = filePath.extension();
				codeFile.name = filePath.stem();
				codeFile.open = true;
				codeFile.change = false;
				codeFile.selected = false;
				ReadFile(codeFile);
				m_CodeFiles.push_back(std::move(codeFile));
				i++;
			}
		}
	}

	void CodeEditorPanel::Render()
	{
		ImGui::Begin("Scripts");
		ImGui::BeginTabBar("###CodeEditorBar");
		size_t i = 0;
		for (auto& f : m_CodeFiles)
		{
			bool tapOpen = ImGui::BeginTabItem(f.name.string().c_str(), &f.open,(f.change ? ImGuiTabItemFlags_UnsavedDocument : 0));
			bool canChange = true;
			if (tapOpen)
			{
				if (f.selected == false)
				{
					m_TextEditor.SetText(f.data);
					canChange = false;
				}
				else if (f.change)
				{
					f.data = m_TextEditor.GetText();
					f.data.pop_back();
				}
				f.selected = true;

				if (m_TextEditor.IsTextChanged() && canChange)
				{

					f.change = true;
				}
				if (f.change && Input::IsKeyPressed(KeysCode::LeftControl) && Input::IsKeyJustPressed(KeysCode::S))
				{
					OverWriteFile(f);
					f.change = false;
				}
				ImGui::EndTabItem();
			}
			else
				f.selected = false;

			if(!f.open)
				m_CodeFiles.erase(m_CodeFiles.begin() + i);
			i++;
		}
		if (m_CodeFiles.empty())
		{
			m_TextEditor.SetText("");
		}
		m_TextEditor.Render("CodeEditor");
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FOLDDERS_PANEL"))
			{
				auto castedData = reinterpret_cast<const FoldersData*>(payload->Data);
				auto ex = castedData->Path.extension();
				auto& path = castedData->Path;
				auto it = std::find_if(m_CodeFiles.begin(), m_CodeFiles.end(), [&](const CodeFile& f) {
					return (f.path == path);
					});
				if (ex == ".py" && it == m_CodeFiles.end())
				{
					CodeFile codeFile;
					codeFile.path = path;
					codeFile.extension = ex;
					codeFile.name = path.stem();
					codeFile.open = true;
					codeFile.change = false;
					codeFile.selected = false;
					ReadFile(codeFile);
					m_CodeFiles.push_back(std::move(codeFile));
				}
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::EndTabBar();
		ImGui::End();
	}

	void CodeEditorPanel::PushFile(const std::filesystem::path& path)
	{
		auto it = std::find_if(m_CodeFiles.begin(), m_CodeFiles.end(), [&](const CodeFile& f) {
			return (f.path == path);
			});

		if (path.extension() == ".py" && it == m_CodeFiles.end())
		{
			CodeFile codeFile;
			codeFile.path = path;
			codeFile.extension = path.extension();
			codeFile.name = path.stem();
			codeFile.open = true;
			codeFile.change = false;
			codeFile.selected = false;
			ReadFile(codeFile);
			m_CodeFiles.push_back(std::move(codeFile));
		}
	}

	void CodeEditorPanel::OverWriteFile(CodeFile& codeFile)
	{
		std::ofstream file;
		file.open(codeFile.path);
		file << m_TextEditor.GetText();
		file.close();
	}

	void CodeEditorPanel::ReadFile(CodeFile& codeFile)
	{
		std::ifstream stream(codeFile.path);
		std::stringstream strStream;
		strStream << stream.rdbuf();
		codeFile.data = strStream.str();
		stream.close();
	}

	/*Shutdown()*/
}
