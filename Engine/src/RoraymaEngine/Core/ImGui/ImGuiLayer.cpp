#include "ImGuiLayer.hh"

#include <imgui.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../App.hh"

rym::ImGuiLayer::ImGuiLayer() :
	Layer("ImGui Layer")
{
}

void rym::ImGuiLayer::OnStart()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    io.Fonts->AddFontDefault();
    io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/theme/fonts/Roboto-Medium.ttf", 15.0f);
    static const ImWchar aws_icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    //static const ImWchar ke_icons_ranges[] = { ICON_MIN_KI, ICON_MAX_KI, 0 };
    ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
    io.Fonts->AddFontFromFileTTF("assets/theme/fonts/fa-solid-900.ttf", 15.0f, &icons_config, aws_icons_ranges);
    //io.Fonts->AddFontFromFileTTF("assets/theme/fonts/kenney-icon-font.ttf", 16.0f, &icons_config, ke_icons_ranges);
    //IM_ASSERT(font != NULL);

    rym::App& app = rym::App::GetInstance();
    GLFWwindow* window = app.GetWindow()->GetContextWindow();

    /* Ventana */
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
}

void rym::ImGuiLayer::OnQuit()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void rym::ImGuiLayer::OnEvent(const Event& events)
{
    if (m_BlockEvents)
    {
        ImGuiIO& io = ImGui::GetIO();
    }
}

void rym::ImGuiLayer::Begin()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

}

void rym::ImGuiLayer::End()
{
    // Rendering
    ImGuiIO& io = ImGui::GetIO();
    App& app = App::GetInstance();
    auto [width, height] = app.GetWindow()->GetWindowSize();
    io.DisplaySize = ImVec2(float(width), float(height));

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}
