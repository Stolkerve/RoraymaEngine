
#include "FoldersPanel.hh"
#include "../CustomImGuiWidgets.hh"

#include <RoraymaEngine/Utils/FileSystem.hh>

#include <algorithm>
#include <cstdio>
#include <fstream>
#include <string>

namespace rym
{
	// TODO: std::filesystem::is_directory is so expensive. Use only for one call function, not inside loops

	void FoldersPanel::IterateNodeFolder(const std::shared_ptr<NodeFolder>& node)
	{
		auto capacity = (std::size_t)std::distance(
			std::filesystem::directory_iterator{ node->Data.Path },
			std::filesystem::directory_iterator{});

		//node->Reserve(capacity);

		size_t counter = 0;
		// Look for a new file

		for (const auto& entry : std::filesystem::directory_iterator(node->Data.Path,
			std::filesystem::directory_options::skip_permission_denied))
		{
			// Ignore the existing files
			auto it = std::find_if(node->begin(), node->end(), [&entry](const std::shared_ptr<NodeFolder>& n) {
				return n->Data.Path == entry.path();
				});

			if (it != node->end())
			{
				// files ignored
			}
			else
			{
				node->AddChild(std::make_shared<NodeFolder>());
				auto& last = node->Back();
				last->Data.Path = entry.path();
				last->Data.isDirectory = std::filesystem::is_directory(last->Data.Path);
				last->Parent = node;
			}
			counter++;
		}

		// Deleted deleted files (Outsize of this context)
		auto it = std::find_if(node->begin(), node->end(), [&](const std::shared_ptr<NodeFolder>& n) {
			return !std::filesystem::exists(n->Data.Path);
			});
		if (it != node->end())
		{
			node->DeleteChild(it);
		}

		// Deleted files (Peticion of the user)
		if (node->Data.NeedDelete)
		{
			//auto de = node->Data.Path == m_DeletedItem.FilePath

			auto it = std::find_if(node->begin(), node->end(), [&](const std::shared_ptr<NodeFolder>& n) {
				return (n->Data.Path == m_DeletedItem.FilePath);
				});


			if (it != node->end())
			{
				auto& a = *it;
				node->DeleteChild(it);
			}

			node->Data.NeedDelete = false;
		}

		std::stable_partition(node->begin(), node->end(), [](const std::shared_ptr<NodeFolder>& n1) {
			return n1->Data.isDirectory;
			});

		node->Data.NeedUpdate = false;
	}

	FoldersPanel::FoldersPanel()
	{
		//RYM_INFO(std::filesystem::current_path().native());
		RYM_INFO(std::filesystem::current_path().relative_path().string());
		// Init extencions map
		m_Extencions[".rym"] = Extencions::rym;

		m_Extencions[".glsl"] = Extencions::code;
		m_Extencions[".h"] = Extencions::code;
		m_Extencions[".h++"] = Extencions::code;
		m_Extencions[".hh"] = Extencions::code;
		m_Extencions[".hpp"] = Extencions::code;
		m_Extencions[".cpp"] = Extencions::code;
		m_Extencions[".cxx"] = Extencions::code;
		m_Extencions[".inl"] = Extencions::code;
		m_Extencions[".py"] = Extencions::code;
		m_Extencions[".pyd"] = Extencions::code;
		m_Extencions[".cs"] = Extencions::code;
		m_Extencions[".sh"] = Extencions::code;
		m_Extencions[".bat"] = Extencions::code;
		m_Extencions[".lua"] = Extencions::code;
		m_Extencions[".as"] = Extencions::code;

		m_Extencions[".png"] = Extencions::image;
		m_Extencions[".jpg"] = Extencions::image;
		m_Extencions[".jpeg"] = Extencions::image;
		m_Extencions[".bmp"] = Extencions::image;

		m_Extencions[".mp3"] = Extencions::music;
		m_Extencions[".ogg"] = Extencions::music;

		m_Extencions[".git"] = Extencions::git;

		m_Extencions[".txt"] = Extencions::text;
		m_Extencions[".yaml"] = Extencions::text;
		m_Extencions[".ini"] = Extencions::text;
		m_Extencions[".json"] = Extencions::text;
		m_Extencions[".log"] = Extencions::text;

		m_Root = std::make_shared<NodeFolder>();
		m_Root->Data.Path = FileSystem::GetWorkSpacePath();
		m_Root->AddChild(std::make_shared<NodeFolder>());

		auto& last = m_Root->Back();
		last->Data.Path = FileSystem::GetWorkSpacePath();
		last->Data.isDirectory = true;
		last->Parent = m_Root;

		IterateNodeFolder(last);
	}



	std::string FoldersPanel::IconFilter(const std::filesystem::path& path)
	{
		Extencions ex;
		const auto it = m_Extencions.find(path.extension().string());
		if (it != m_Extencions.end())
			ex = m_Extencions[path.extension().string()];
		else
			ex = Extencions::unknow;

		switch (ex)
		{
		case Extencions::code: return ICON_FA_FILE_CODE; break;
		case Extencions::image: return ICON_FA_FILE_IMAGE; break;
		case Extencions::git: return ICON_FA_CODE_BRANCH; break;
		case Extencions::music: return ICON_FA_FILE_AUDIO; break;
		case Extencions::rym: return ICON_FA_GAMEPAD; break;
		case Extencions::text: return ICON_FA_FILE_SIGNATURE; break;
		default: return ICON_FA_FILE_ALT; break;
		}
	}

	/*
	make a enum with the extencion casted to int
	*/

	void FoldersPanel::Render()
	{
		ImGui::Begin(ICON_FA_FOLDER_OPEN" Folders Panel");

		int i = 0;
		for (auto& c : *m_Root)
		{
			DrawNode(c, i);
			i++;
		}

		ImGui::End();
	}

	void FoldersPanel::Refresh()
	{
		IterateNodeFolder(m_Root->Back());
	}

	void FoldersPanel::DrawNode(std::shared_ptr<NodeFolder>& node, int id)
	{
		ImGui::PushID((size_t)&node);

		static const std::filesystem::path* droppedItem = nullptr;
		static const std::filesystem::path* draggedItem = nullptr;

		const std::string sfilename = node->Data.Path.filename().string();
		const std::string folderName(ICON_FA_FOLDER"  "+ sfilename);
		const std::string fileName(IconFilter(sfilename) + "  " + sfilename);

		const ImGuiTreeNodeFlags flags =
			(m_SelectedItem == node.get() ? ImGuiTreeNodeFlags_Selected : 0) |
			(!node->Data.isDirectory ? ImGuiTreeNodeFlags_Leaf : 0) |
			ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_FramePadding;
		bool opened = ImGui::TreeNodeEx(reinterpret_cast<void*>(id), flags,
				!node->Data.isDirectory ?
				fileName.c_str() : folderName.c_str());

		if (ImGui::IsItemToggledOpen() && node->Data.isDirectory)
		{
			//RYM_INFO(node->Data.Path.filename().string());
			if (node->Data.Open)
			{
				node->Data.Open = false;
			}
			else
			{
				node->Data.Open = true;
				node->Data.NeedUpdate = true;
			}
		}

		if (ImGui::IsItemClicked())
		{
			m_SelectedItem = node.get();
		}


		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
		{
			m_HoveredItem = node.get();
			if (ImGui::IsMouseDoubleClicked(0))
			{
				if (m_HoveredItem->Data.Path.extension() == ".rym")
				{
					m_ChangeSceneFunc(m_HoveredItem->Data.Path.string());
					RYM_INFO("Scene xd {}", m_HoveredItem->Data.Path.string());
				}
			}
		}

		ImGui::PushID(id);
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceNoHoldToOpenOthers | ImGuiDragDropFlags_SourceNoDisableHover))
		{
			ImGui::SetDragDropPayload("FOLDDERS_PANEL", &m_SelectedItem->Data, sizeof(FoldersData));
			const auto& hoverPath = m_HoveredItem->Data.Path;
			const auto& itemPath = m_SelectedItem->Data.Path;
			std::string text("Moveing ");
			if (m_HoveredItem->Data.isDirectory)
			{
				text.append(itemPath.filename().string() + " to ");
				text.append(hoverPath.filename().string() + " folder");
			}
			else
			{
				text.append(itemPath.filename().string() + " is not allow here");
			}
			ImGui::Text(text.c_str());
			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget() && m_HoveredItem)
		{
			if (m_HoveredItem->Data.isDirectory && (m_SelectedItem->Data.Path.parent_path() != m_HoveredItem->Data.Path))
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FOLDDERS_PANEL"))
				{
					// TODO: use payload, now its work
					if (FileSystem::Move(m_SelectedItem->Data.Path, m_HoveredItem->Data.Path))
					{
						m_HoveredItem->Data.NeedUpdate = true;

						auto& selectedParent = m_SelectedItem->Parent;
						selectedParent->Data.NeedUpdate = true;
						selectedParent->Data.NeedDelete = true;
						//m_Root->Data.NeedUpdate = true;
						//m_DeletedItem.ParentPath = draggedItem->parent_path();
						m_DeletedItem.FilePath = m_SelectedItem->Data.Path;
					}
				}
				ImGui::EndDragDropTarget();
			}
		}
		ImGui::PopID();

		if (ImGui::BeginPopupContextItem())
		{
			// the cursor can be outsize of the original hovered item
			static NodeFolder* lastHovered;
			static bool isEdited = false;
			static bool cancelDelete = false;
			static bool okDelete = false;
			static bool askTrash = false;
			lastHovered = m_HoveredItem;
			//RYM_INFO(lastHovered->Path.filename().string());
			if (lastHovered->Data.Path != FileSystem::GetWorkSpacePath())
			{
				//ImGui::PushItemWidth(ImGui::CalcItemWidth() / 2);

				if (ImGui::Selectable("New folder", false, ImGuiSelectableFlags_DontClosePopups))
				{
					ImGui::OpenPopup("Morelater");
				}

				if (ImGui::Selectable("New file", false, ImGuiSelectableFlags_DontClosePopups))
				{
					ImGui::OpenPopup("Morelater");
				}
				bool lok;
				CreateModalOk("Morelater", "More later", &lok);

				if (ImGui::Selectable("Move to trash", false, ImGuiSelectableFlags_DontClosePopups))
				{
					if (lastHovered->Data.Open)
					{
						ImGui::OpenPopup("CloseFolder");
					}
					else
						ImGui::OpenPopup("Delete?");
				}
				static bool ok = false;
				CreateModalOk("CloseFolder", "Please close the target folder", &ok);
				CreateModalOkCancel("Delete?", "All thoses files going to be move to the recicle bin\n\n", &okDelete, &cancelDelete);
				if (okDelete)
				{
					lastHovered->Data.Open = false;
					auto& parent = lastHovered->Parent;
					parent->Data.NeedDelete = true;
					parent->Data.NeedUpdate = true;
					m_DeletedItem.FilePath = lastHovered->Data.Path;

					FileSystem::MoveToReciclyBin(m_DeletedItem.FilePath);

					okDelete = false;
					ImGui::CloseCurrentPopup();
				}

				/* Rename */
				char buffer[35];
				std::strcpy(buffer, lastHovered->Data.Path.filename().string().c_str());
				if (ImGui::InputText("###name", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsNoBlank))
				{
					RYM_INFO("Nuevo nombre: {}", std::string(buffer));
					auto v = std::getenv("HOME");
					if(v)
						RYM_INFO(std::string(v));
					//m_EntitySelected->Tag = std::string(buffer);
					auto newName = (lastHovered->Data.Path.parent_path().string() + "\\" + std::string(buffer));
					bool renameSucces = std::rename(lastHovered->Data.Path.string().c_str(),
						newName.c_str());
					lastHovered->Data.Path = newName;
					ImGui::CloseCurrentPopup();
				}
				//ImGui::PopItemWidth();
			}
			ImGui::EndPopup();
		}

		if (node->Data.Open)
		{
			int id = 0;
			for (auto& c : *node)
			{
				DrawNode(c, id);
				id++;
			}
			if (node->Data.NeedUpdate || timer >= 150.0f) {
				IterateNodeFolder(node);
				timer = 0.f;
			}
		}

		if (opened)
			ImGui::TreePop();
		ImGui::PopID();

		timer += 1.f / 60.f;
	}
};
