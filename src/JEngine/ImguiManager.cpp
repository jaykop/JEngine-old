#include "ImguiManager.h"
#include "Engine.h"
#include "glew.h"
#include "imgui.h"
#include "imgui_impl_sdl_gl3.h"

JE_BEGIN

SDL_Window* ImguiManager::m_pWindow = nullptr;

void ImguiManager::Init(SDL_Window* _window)
{
	if (JENGINE::m_IMGUI) {
		m_pWindow = _window;
		ImGui_ImplSdlGL3_Init(_window);
	}
}

void ImguiManager::EventUpdate(SDL_Event* _event)
{
	if (JENGINE::m_IMGUI)
		ImGui_ImplSdlGL3_ProcessEvent(_event);
}

void ImguiManager::Update()
{
	if (JENGINE::m_IMGUI) {
		
		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImColor(114, 144, 154);

		ImGui_ImplSdlGL3_NewFrame(m_pWindow);

		// 1. Show a simple window.
		// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
		{
			static float f = 0.0f;
			ImGui::Text("Hello, world!");                           // Some text (you can use a format string too)
			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float as a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats as a color
			if (ImGui::Button("Demo Window"))                       // Use buttons to toggle our bools. We could use Checkbox() as well.
				show_demo_window ^= 1;
			if (ImGui::Button("Another Window"))
				show_another_window ^= 1;
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}

		// 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name the window.
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);
			ImGui::Text("Hello from another window!");
			ImGui::End();
		}

		// 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow().
		if (show_demo_window)
		{
			ImGui::ShowDemoWindow(&show_demo_window);
			ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
		}

		// Rendering
		//static int s_left = (int)ImGui::GetIO().DisplaySize.x / 2, s_right = -s_left, s_top = (int)ImGui::GetIO().DisplaySize.y / 2, s_bottom = -s_bottom;
		glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
		//glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		//glClear(GL_COLOR_BUFFER_BIT);
		ImGui::Render();
	}
}

void ImguiManager::Close()
{
	if (JENGINE::m_IMGUI) 
		ImGui_ImplSdlGL3_Shutdown();
}

JE_END