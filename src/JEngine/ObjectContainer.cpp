#include "Object.h"
#include "ObjectContainer.h"
#include "ComponentManager.h"
#include "imgui.h"
#include "ImguiManager.h"

JE_BEGIN

ObjectContainer::~ObjectContainer()
{
	ClearObjectMap();
}

void ObjectContainer::RemoveObject(const char* _name)
{
	// Find the one to remove
	auto toRemove = m_objectMap.find(_name);

	// If found the one
	if (toRemove != m_objectMap.end()) {
		delete (*toRemove).second;
		(*toRemove).second = nullptr;
	}

	else
		JE_DEBUG_PRINT("!ObjectContainer - No such name of enrolled object: %s\n", _name);
}

Object* ObjectContainer::GetObject(const char * _name)
{
	// Find the one to remove
	auto faound = m_objectMap.find(_name);

	// If found the one
	if (faound != m_objectMap.end())
		return (*faound).second;

	else {
		JE_DEBUG_PRINT("!ObjectContainer - No such name of enrolled object: %s\n", _name);
		return nullptr;
	}
}

ObjectMap& ObjectContainer::GetObjectMap()
{
	return m_objectMap;
}

bool ObjectContainer::HasObject(const char* _name)
{
	// Find the one to remove
	auto faound = m_objectMap.find(_name);

	// If found the one
	if (faound != m_objectMap.end()) {
		return true;
	}

	return false;
}

void ObjectContainer::ClearObjectMap()
{
	for (auto obj : m_objectMap)
	{
		if (obj.second) {
			delete obj.second;
			obj.second = nullptr;
		}
	}

	m_objectMap.clear();
}

void ObjectContainer::EditorUpdate(const float _dt)
{
	//ImGui::Begin("StateManager");
	//ImGui::Text("*The Number of Objects: %d", m_pOBC->GetObjectMap().size());
	//static char ObjId[128] = "ObjectName";
	//ImGui::InputText("", ObjId, IM_ARRAYSIZE(ObjId));
	//ImGui::SameLine();
	//if (ImGui::Button("Search")) {
	//	foundObject = m_pOBC->HasObject(ObjId);
	//	s_pObj = m_pOBC->GetObject(ObjId);
	//}

	//ImGui::End();

	//if (foundObject)
	//{
	//	ImGui::Begin(ObjId);
	//	ImGui::Text("*Object Id: %d", s_pObj->GetId());
	//	if (s_pObj->GetParent())
	//		ImGui::Text("*Parent Object: %s", s_pObj->GetParent()->GetName().c_str());
	//	ImGui::Text("*Component List:");
	//	/*for (auto component : s_pObj->GetComponentMap())
	//	ImGui::Button(component->);*/
	//	ImGui::End();
	//}
}

JE_END

