#include "EditorLayer.hh"

EditorLayer::EditorLayer() : Layer("Editor_Layer") {}

void EditorLayer::OnStart()
{
    auto& app = App::GetInstance();
    const auto&[width, height] = app.GetWindow()->GetWindowSize();
    //m_FrameBuffer = std::make_shared<FrameBuffer>(width, height,
    //   { Attachment::RGBA8 });
    m_FrameBuffer = std::make_shared<FrameBuffer>(width, height);
	m_Scenes.push_back(std::make_shared<Scene>("TestScene"));
	//m_SelectedScene = &m_Scenes.back(); // More later
    m_EntitysPanel.SetContext(m_Scenes.back());

	// Buhhhh use std::bind. Labmdas > std::bind
	m_FoldersPanel.SetChangeSceneCallBack([&](const std::string& path) {
		OpenScene(path);
		});

	CodeEditorPanel::Init();
}

void EditorLayer::OnEvent(const Event& event)
{

}

void EditorLayer::RenderCartesianPlane()
{
	constexpr float maxFloat = std::numeric_limits<float>().max();

    Renderer::SetLineWidth(1.4f);
    Renderer2D::BeginWire(m_EditorCamera);
	Renderer2D::DrawLine({ {-maxFloat, 0.f}, {maxFloat, 0.f} }, Color::GREEN, -21); // X axis
	Renderer2D::DrawLine({ {0.f, -maxFloat}, {0.f, maxFloat} }, Color::RED, -21); // Y axis
    Renderer2D::EndWire();
	Renderer::SetLineWidth(1.0f);
}

void EditorLayer::CalculateDeltaWorlMouse()
{
	const auto mouse = CaculateMouseViewport();
	m_InitWorldMouse = glm::unProject(
		glm::vec3(mouse.x, m_ViewportSize.y - mouse.y, 1.f),
		m_EditorCamera.GetViewMatrix(),
		m_EditorCamera.GetProjectionMatrix(),
		glm::vec4(0.f, 0.f, m_ViewportSize.x, m_ViewportSize.y));
	m_DeltaWorldMouse = m_InitWorldMouse - m_LastWorldMouse;
	m_LastWorldMouse = m_InitWorldMouse;
}

glm::vec2 EditorLayer::CaculateMouseViewport()
{
    glm::vec2 m = { ImGui::GetMousePos().x, ImGui::GetMousePos().y };
    m -= m_ViewportPos;
    return m;
}

void EditorLayer::MoveEntity(const std::shared_ptr<Entity>& entity, float delta)
{
    if (entity->HaveComponent<SpriteComponent>())
    {
        const auto transformComp = entity->GetComponent<TransformComponent>();
        transformComp->translation += glm::vec2(m_DeltaWorldMouse.x, m_DeltaWorldMouse.y);
    }
}

void EditorLayer::ChangeScene(const std::shared_ptr<Scene>& newScene)
{
	m_Scenes.back() = newScene;
	m_EntitysPanel.SetContext(m_Scenes.back());
	m_EntitysPanel.EntityClicked(nullptr);
}

void EditorLayer::ChangeScene(std::shared_ptr<Scene>&& newScene)
{
	m_Scenes.back() = std::move(newScene);
	m_EntitysPanel.SetContext(m_Scenes.back());
	m_EntitysPanel.EntityClicked(nullptr);
}

void EditorLayer::OpenScene(const std::vector<std::string>& scenesPath)
{
	ChangeScene(std::make_shared<Scene>());
	SceneSerializer::SceneDesSerialize(m_Scenes.back(), scenesPath.back());
	m_Scenes.back()->OnViewportResize(uint32_t(m_ViewportSize.x), uint32_t(m_ViewportSize.y));
}

void EditorLayer::OpenScene(const std::string& scenePath)
{
	ChangeScene(std::make_shared<Scene>());
	SceneSerializer::SceneDesSerialize(m_Scenes.back(), scenePath);
	m_Scenes.back()->OnViewportResize(uint32_t(m_ViewportSize.x), uint32_t(m_ViewportSize.y));
}

void EditorLayer::SaveScene(const std::string& scenePath)
{
	constexpr const char* rymExtension = ".rym";
	if (!scenePath.empty())
	{
		SceneSerializer::SceneSerialize(m_Scenes.back(), scenePath + rymExtension);
	}
}

void EditorLayer::UpdateEditorMode(float _delta)
{
    // Update editor camera
    if (m_ViewportHovered)
        m_EditorCamera.Update(m_ViewportSize);

    // Mouse picking relate
    m_FrameBuffer->ClearEntitysAttachment();

	// Draw dynamic grid
	//m_DynamicGrid.Render(m_EditorCamera);

    // Draw cartesanian draw
    RenderCartesianPlane();

    //Update the scene instace
	m_Scenes.back()->OnUpdateEditor(m_EditorCamera);

    // Picking
    auto m = CaculateMouseViewport();
    m.y = m_ViewportSize.y - m.y; //Flip y axis
    static std::shared_ptr<Entity> entityPtrSelected = nullptr;

	static bool canDrawOverlay = false;

	if (canDrawOverlay)
	{
		m_SEOVerlay.DrawOverlay(entityPtrSelected, m_EditorCamera);
	}
    if (m.x > 0 && m.x < m_ViewportSize.x && m.y > 0 && m.y < m_ViewportSize.y &&
        m_ViewportFocused)
    {
		//int IDSelected = m_FrameBuffer->ReadEntitys(int(m.x), int(m.y));
		//RYM_INFO(IDSelected);
        if (Input::IsButtonJustPressed(MouseCode::ButtonLeft))
        {
            int entityIDSelected = m_FrameBuffer->ReadEntitys(int(m.x), int(m.y));
            if (entityIDSelected >= 0) //&& entityPtrSelected == nullptr
            {
                entityPtrSelected = m_Scenes.back()->GetEntity(entityIDSelected);
                m_EntitysPanel.EntityClicked(entityPtrSelected);
            }
			else
			{
				canDrawOverlay = false;
				entityPtrSelected = nullptr;
			}
        }

        if (entityPtrSelected != nullptr)
        {
			canDrawOverlay = true;
			if (Input::IsButtonPressed(MouseCode::ButtonLeft))
			{
				m_EntitysPanel.EntityClicked(entityPtrSelected);

				// Move the entity along the mouse
				MoveEntity(entityPtrSelected, _delta);
			}
        }
		else
		{
			canDrawOverlay = false;
            entityPtrSelected = nullptr;
		}
    }
}

void EditorLayer::UpdateGameMode(float _delta)
{
	m_Scenes.back()->OnUpdateGame(_delta);
}

void EditorLayer::OnUpdate(float _delta)
{
	CalculateDeltaWorlMouse();

    // Rendering stuff
	if (m_ViewportSize.x >= 0 && m_ViewportSize.y >= 0 && m_NeedResize)
	{
		m_NeedResize = false;
        m_FrameBuffer->Resize(uint32_t(m_ViewportSize.x), uint32_t(m_ViewportSize.y));
	}
    m_FrameBuffer->Bind();
    Renderer::Clear({0.07f, 0.07f, 0.07f, 1.f});

    switch (m_LayerMode)
    {
    case rym::LayerMode::EDITOR_MODE: UpdateEditorMode(_delta); break;
    case rym::LayerMode::GAME_MODE: UpdateGameMode(_delta); break;
    default: break;
    }

    m_FrameBuffer->UnBind();
}

void EditorLayer::OnQuit()
{
	RYM_INFO("cerrando la escena: {}", this->GetName());
}

void EditorLayer::OnImGui()
{
	auto& statistics = BatchStatistics::Get();

    static bool dockspaceOpen = true;
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
    float minWinSizeX = style.WindowMinSize.x;
    style.WindowMinSize.x = 175.0f;
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    style.WindowMinSize.x = minWinSizeX;

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu(ICON_FA_COGS"  Options"))
        {
            if (ImGui::MenuItem(ICON_FA_FOLDER_OPEN"  Open scene", "Ctrl + O"))
            {
                auto scenePath = pfd::open_file("Scenes", "assets/scenes",
                    { "Scene File (.rym)", "*.rym" }).result();
				if(!scenePath.empty())
					OpenScene(scenePath);
            }
            if (ImGui::MenuItem(ICON_FA_SAVE"  Save as...", "Ctrl + S"))
            {
                auto scenePath = pfd::save_file("Scenes", "assets/scenes", { "Scene File (.rym)", "*.rym" }).result();
				SaveScene(scenePath);
            }

            if (ImGui::MenuItem(ICON_FA_TIMES_CIRCLE"  Exit"))
            {
                App::GetInstance().ShutDown();
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    // Panels
    {
        m_EntitysPanel.Render();
        m_FoldersPanel.Render();
        //ConsolePanel::GetInstace()->Render();
        m_RenderingInfoPanel.Render();
		CodeEditorPanel::Render();
    }

    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });

        // Scene
        auto sceneName = (m_Scenes.back()->Name + "###");
        ImGui::Begin("Scenes");

        ImGui::Spacing();

		static bool isPlay = false;
        if (ImGui::Button(ICON_FA_PLAY"  Play scene"))
        {
            m_LayerMode = LayerMode::GAME_MODE;
			if (!isPlay)
				m_Scenes.back()->OnStartGame();
			isPlay = true;
        }
        ImGui::SameLine();
        if(ImGui::Button(ICON_FA_STOP"  Stop scene"))
        {
            m_LayerMode = LayerMode::EDITOR_MODE;
			if(isPlay)
				m_Scenes.back()->OnQuitGame();
			isPlay = false;
        }
		ImGui::Separator();

		static bool tap = true;
		ImGui::BeginTabBar("ScenesBar");
		ImGui::BeginTabItem(sceneName.c_str(), &tap);
        // Viewport
        m_ViewportPos = {
            // Global start point the window position and the tap bar offset
            ImGui::GetWindowPos().x - ImGui::GetCursorPos().x,
            ImGui::GetWindowPos().y + ImGui::GetCursorPos().y
        };

        glm::vec2 viewportPanelSize = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };
        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();
        m_ViewportDocked = ImGui::IsWindowDocked();

        if (m_ViewportSize != viewportPanelSize)
        {
            m_EditorCamera.Resize(viewportPanelSize.x, viewportPanelSize.y);
			m_Scenes.back()->OnViewportResize(uint32_t(viewportPanelSize.x), uint32_t(viewportPanelSize.y));
            m_ViewportSize = viewportPanelSize;
			m_NeedResize = true;
        }

        uint32_t sceneTextID = m_FrameBuffer->GetColorAttachment();
        ImGui::Image(reinterpret_cast<void*>(sceneTextID), { m_ViewportSize.x, m_ViewportSize.y }, { 0, 1 }, { 1, 0 });

		// gizmos
		/*
		{
			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			//TODO: multiple the vertices by the viewmatrix of the editor camera
			draw_list->AddRectFilled({ 100.f, 100.f }, { 200.f, 200.f }, ImColor({ 1.0f, 1.0f, 0.4f, 1.0f }));
		}
		*/

		ImGui::EndTabItem();
		ImGui::EndTabBar();

        ImGui::End();
        ImGui::PopStyleVar();
    }

    ImGui::End();

    //ImGui::ShowDemoWindow();
}
