#include "imgui.h"
#include "imgui_impl_sdl_gl3.h"
#include "glew.h"
#include "Application.h"
#include "ImguiManager.h"
#include "GLManager.h"
#include "Component.h"

jeBegin

SDL_Window*					IMGUI::m_pWindow = nullptr;
IMGUI::EditorList			IMGUI::m_editors;
IMGUI::ObjectEditorMap		IMGUI::m_objEditors;
IMGUI::ComponentEditorMap	IMGUI::m_cptEditors;

void ImguiManager::AddEditorFunc(const EditorUpdateFunc &_pFunc)
{
	m_editors.push_back(_pFunc);
}

void ImguiManager::AddComponentEditor(Component* _component)
{
	m_cptEditors.push_back(_component);
}

void ImguiManager::RemoveComponentEditor(Component* _component)
{
	for (auto component = m_cptEditors.begin();
		component != m_cptEditors.end(); ++component) {
		if (*component == _component) {
			m_cptEditors.erase(component);
			break;
		}
	}
}

void ImguiManager::AddObjectEditor(Object* _object)
{
	m_objEditors.push_back(_object);
}

void ImguiManager::RemoveObjectEditor(Object* _object)
{
	for (auto object = m_objEditors.begin();
		object != m_objEditors.end(); ++object) {
		if (*object == _object) {
			m_objEditors.erase(object);
			break;
		}
	}
}

void ImguiManager::ClearComponentEditor()
{
	if (!m_cptEditors.empty())
		m_cptEditors.clear();
}

void ImguiManager::ClearObjectEditor()
{
	if (!m_objEditors.empty())
		m_objEditors.clear();
}

bool ImguiManager::Init(SDL_Window* _window)
{
	if (APP::m_IMGUI) {
		m_pWindow = _window;
		bool result = ImGui_ImplSdlGL3_Init(_window);
		if (!result)
			jeDebugPrint("!ImguiManager: Could not initialize IMGUI.\n");
		return result;
	}

	return true;
}

void ImguiManager::EventUpdate(SDL_Event* _event)
{
	if (APP::m_IMGUI)
		ImGui_ImplSdlGL3_ProcessEvent(_event);
}

void ImguiManager::Update(const float _dt)
{
	if (APP::m_IMGUI) {

		ImVec4 clear_color = ImColor(114, 144, 154);
		ImGui_ImplSdlGL3_NewFrame(m_pWindow);

		//TODO Add...
		//// System Manager
		//{
		//	ImGui::Begin("System Manager");
		//	ImGui::Text("*Coming Soon...");
		//	ImGui::End(); 
		//}

		//// Json Exporter
		//{
		//	ImGui::Begin("Json Exporter");
		//	ImGui::Text("*Coming Soon...");
		//	ImGui::End();
		//}
		// Updated added editor functions
		for (auto editorUpdate : m_editors)
			editorUpdate(_dt);

		// Update object's eidtor window
		for (auto objEditor : m_objEditors)
			objEditor->EditorUpdate(_dt);

		// Update component's eidtor window
		for (auto componentEditor : m_cptEditors)
			componentEditor->EditorUpdate(_dt);

		// Rendering
		glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
		ImGui::Render();
	}
}

void ImguiManager::Close()
{
	if (APP::m_IMGUI)
		ImGui_ImplSdlGL3_Shutdown();
}

//// 1. Show a simple window.
//// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
//{
//	static float f = 0.0f;
//	ImGui::Text("Hello, world!");                           // Some text (you can use a format string too)
//	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float as a slider from 0.0f to 1.0f
//	ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats as a color
//	if (ImGui::Button("Demo Window"))                       // Use buttons to toggle our bools. We could use Checkbox() as well.
//		show_demo_window ^= 1;
//	if (ImGui::Button("Another Window"))
//		show_another_window ^= 1;
//	
//}
//
//// 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name the window.
//if (show_another_window)
//{
//	ImGui::Begin("Another Window", &show_another_window);
//	ImGui::Text("Hello from another window!");
//	ImGui::End();
//}
//
//// 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow().
//if (show_demo_window)
//{
//	ImGui::ShowDemoWindow(&show_demo_window);
//	ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
//}

jeEnd
