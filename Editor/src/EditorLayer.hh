#pragma once

#include <RoraymaEngine/RoraymaEngine.hh>
#include <RoraymaEngine/Renderer/DynamicGrid.hh>

#include <Portable_File_Dialogs/portable-file-dialogs.h>
#include <pybind11/embed.h>

#include "Panels/RenderingInfoPanel.hh"
#include "Panels/EntitysPanel.hh"
#include "Panels/ConsolePanel.hh"
#include "Panels/FoldersPanel.hh"
#include "Panels/CodeEditorPanel.hh"

#include "Renderer/SelectedEntityOverlay.hh"

using namespace rym;
using namespace api;

class EditorLayer : public Layer
{
public:
	EditorLayer();

	virtual void OnStart() override;
	virtual void OnEvent(const Event& event) override;
	virtual void OnUpdate(float _delta) override;
	virtual void OnQuit() override;
	virtual void OnImGui() override;

private:
	void UpdateEditorMode(float _delta);
	void UpdateGameMode(float _delta);
	void RenderCartesianPlane();
	void CalculateDeltaWorlMouse();
	void MoveEntity(const std::shared_ptr<Entity>& entity, float delta);
	void OpenScene(const std::vector<std::string>& scenesPath);
	void OpenScene(const std::string& scenePath);
	void SaveScene(const std::string& scenePath);
	void ChangeScene(const std::shared_ptr<Scene>& newScene);
	void ChangeScene(std::shared_ptr<Scene>&& newScene);
	glm::vec2 CaculateMouseViewport();

	std::shared_ptr<Scene>* m_SelectedScene;
	std::shared_ptr<FrameBuffer> m_FrameBuffer;
	std::vector<std::shared_ptr<Scene>> m_Scenes;

	//DynamicGrid m_DynamicGrid;

	EditorCamera m_EditorCamera;
	SEOverlay m_SEOVerlay;

	RenderingInfoPanel m_RenderingInfoPanel;
	EntitysPanel m_EntitysPanel;
	FoldersPanel m_FoldersPanel;

	LayerMode m_LayerMode = LayerMode::EDITOR_MODE;
	glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
	glm::vec2 m_ViewportPos = { 0.0f, 0.0f };

	glm::vec3 m_LastMouseViewport = { 0.f, 0.f, 0.f };
	glm::vec3 m_FirstMouseViewport = { 0.f, 0.f, 0.f };
	glm::vec3 m_MouseViewportDelta = { 0.f, 0.f, 0.f };

	bool m_ViewportFocused;
	bool m_ViewportHovered;
	bool m_ViewportDocked;
	bool m_NeedResize;
	bool m_ScenesOpen = true;
	bool m_BlockSelectedEntity = false;
};
