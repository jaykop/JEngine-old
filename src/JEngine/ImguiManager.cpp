#include "imgui.h"
#include "imgui_impl_sdl_gl3.h"
#include "glew.h"
#include "Application.h"
#include "ImguiManager.h"
#include "GLManager.h"
#include "Component.h"
#include "Object.h"

jeBegin

SDL_Window*					IMGUI::pWindow_ = nullptr;
IMGUI::EditorList			IMGUI::editors_;
IMGUI::ObjectEditorMap		IMGUI::objEditors_;
IMGUI::ComponentEditorMap	IMGUI::cptEditors_;

void ImguiManager::AddEditorFunc(const EditorUpdateFunc &_pFunc)
{
	editors_.push_back(_pFunc);
}

void ImguiManager::AddComponentEditor(Component* pComponent)
{
	cptEditors_.push_back(pComponent);
}

void ImguiManager::RemoveComponentEditor(Component* pComponent)
{
	for (auto component = cptEditors_.begin();
		component != cptEditors_.end(); ++component) {
		if (*component == pComponent) {
			cptEditors_.erase(component);
			break;
		}
	}
}

void ImguiManager::AddObjectEditor(Object* pObject)
{
	objEditors_.push_back(pObject);
}

void ImguiManager::RemoveObjectEditor(Object* pObject)
{
	for (auto object = objEditors_.begin();
		object != objEditors_.end(); ++object) {
		if (*object == pObject) {
			objEditors_.erase(object);
			break;
		}
	}
}

void ImguiManager::ClearComponentEditor()
{
	if (!cptEditors_.empty())
		cptEditors_.clear();
}

void ImguiManager::ClearObjectEditor()
{
	if (!objEditors_.empty())
		objEditors_.clear();
}

bool ImguiManager::Init(SDL_Window* pWindow)
{
	if (APP::activateIMGUI_) {
		pWindow_ = pWindow;
		bool result = ImGui_ImplSdlGL3_Init(pWindow);
		if (!result)
			jeDebugPrint("!ImguiManager: Could not initialize IMGUI.\n");
		return result;
	}

	return true;
}

void ImguiManager::EventUpdate(SDL_Event* pEvent)
{
	if (APP::activateIMGUI_)
		ImGui_ImplSdlGL3_ProcessEvent(pEvent);
}

void ImguiManager::Update(float dt)
{
	if (APP::activateIMGUI_) {

		ImVec4 clear_color = ImColor(114, 144, 154);
		ImGui_ImplSdlGL3_NewFrame(pWindow_);

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
		for (auto editorUpdate : editors_)
			editorUpdate(dt);

		// Update object's eidtor window
		for (auto objEditor : objEditors_)
			objEditor->EditorUpdate(dt);

		// Update component's eidtor window
		for (auto componentEditor : cptEditors_)
			componentEditor->EditorUpdate(dt);

		// Rendering
		glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
		ImGui::Render();
	}
}

void ImguiManager::Close()
{
	if (APP::activateIMGUI_)
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
