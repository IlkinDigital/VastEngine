#include "vastpch.h"
#include "ImGuiLayer.h"

#include "Vast/ApplicationCore/Application.h"

#include "FontManager.h"

#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

// TODO: Make platform independent
#include <GLFW/glfw3.h>

#include <ImGuizmo.h>

namespace Vast {

	void ImGuiLayer::OnAttach()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		FontManager::UploadFont("Assets/Fonts/Rubik/Rubik-Regular.ttf", FontWeight::Regular);
		FontManager::UploadFont("Assets/Fonts/Rubik/Rubik-Bold.ttf", FontWeight::Bold);
		FontManager::UploadFont("Assets/Fonts/Rubik/Rubik-Light.ttf", FontWeight::Light);

		FontManager::SetDefault(FontWeight::Regular);

		// Setup Dear ImGui style
		//ImGui::StyleColorsDark();

		SetVastThemeColors();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}


		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnGUIRender()
	{

	}

	void ImGuiLayer::OnEvent(Event& event)
	{
		if (BlockEvents)
		{
			ImGuiIO& io = ImGui::GetIO();
			event.Handled |= event.IsInCategory(EventCategory::Mouse) & io.WantCaptureMouse;
			event.Handled |= event.IsInCategory(EventCategory::Keyboard) & io.WantCaptureKeyboard;
		}
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(static_cast<float>(app.GetWindow().GetWidth()), static_cast<float>(app.GetWindow().GetHeight()));

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

	void ImGuiLayer::SetVastThemeColors()
	{
		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4{ 40.0f / 255.0f, 40.0f / 255.0f, 42.0f / 255.0f, 1.0f };

		// Headers
		colors[ImGuiCol_Header] = ImVec4{ 68.0f / 255.0f, 82.0f / 255.0f, 86.0f / 255.0f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 81.0f / 255.0f, 152.0f / 255.0f, 135.0f / 255.0f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 38.0f / 255.0f, 38.0f / 255.0f, 39.0f / 255.0f, 1.0f };

		// Buttons
		colors[ImGuiCol_Button] = ImVec4{ 10.0f / 255.0f, 117.0f / 255.0f, 94.0f / 255.0f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 71.0f / 255.0f, 170.0f / 255.0f, 148.0f / 255.0f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 38.0f / 255.0f, 38.0f / 255.0f, 39.0f / 255.0f, 1.0f };

		// Frame BG
		colors[ImGuiCol_FrameBg] = ImVec4{ 51.0f / 255.0f, 52.0f / 255.0f, 54.0f / 255.0f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 81.0f / 255.0f, 152.0f / 255.0f, 135.0f / 255.0f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 112.0f / 255.0f, 184.0f / 255.0f, 182.0f / 255.0f, 1.0f };

		// Tabs
		colors[ImGuiCol_Tab] = ImVec4{ 31.0f / 255.0f, 31.0f / 255.0f, 31.0f / 255.0f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 80.0f / 255.0f, 159.0f / 255.0f, 123.0f / 255.0f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 15.0f / 255.0f, 101.0f / 255.0f, 86.0f / 255.0f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 64.0f / 255.0f, 76.0f / 255.0f, 72.0f / 255.0f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 80.0f / 255.0f, 87.0f / 255.0f, 86.0f / 255.0f, 1.0f };

		// Title
		colors[ImGuiCol_TitleBg] = ImVec4{ 40.0f / 255.0f, 40.0f / 255.0f, 42.0f / 255.0f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 40.0f / 255.0f, 40.0f / 255.0f, 42.0f / 255.0f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 80.0f / 255.0f, 135.0f / 255.0f, 124.0f / 255.0f, 1.0f };
	}

}