#include "Object.h"
#include "ObjectContainer.h"
#include "imgui.h"
#include "ImguiManager.h"

JE_BEGIN

ObjectContainer* OBJECT::m_pSharedContainer = nullptr;

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
		IMGUI::RemoveObjectEditor(toRemove->second);
		delete toRemove->second;
		toRemove->second = nullptr;
	}

	else
		JE_DEBUG_PRINT("!ObjectContainer - No such name of enrolled object: %s\n", _name);
}

void ObjectContainer::RemoveObject(unsigned _id)
{
	bool notFound = true;
	for (auto object : m_objectMap) {
		if (_id == object.second->GetId()) {
			IMGUI::RemoveObjectEditor(object.second);
			delete object.second;
			notFound = false;
			object.second = nullptr;
		}
	}

	if (notFound)
		JE_DEBUG_PRINT("!ObjectContainer - No such name of enrolled object: %i\n", _id);
}

	Object* ObjectContainer::GetObject(const char *_name)
{
	// Find the one to remove
	auto found = m_objectMap.find(_name);

	// If found the one
	if (found != m_objectMap.end())
		return found->second;

	JE_DEBUG_PRINT("!ObjectContainer - No such name of enrolled object: %s\n", _name);
	return nullptr;
	
}

Object* ObjectContainer::GetObject(unsigned _id)
{
	for (auto object : m_objectMap) {
		if (_id == object.second->GetId())	{
			return object.second;
		}
	}

	JE_DEBUG_PRINT("!ObjectContainer - No such name of enrolled object: %i\n", _id);
	return nullptr;
}

ObjectMap& ObjectContainer::GetObjectMap()
{
	return m_objectMap;
}

bool ObjectContainer::HasObject(const char* _name)
{
	// Find the one to remove
	auto found = m_objectMap.find(_name);

	// If found the one
	if (found != m_objectMap.end()) 
		return true;

	JE_DEBUG_PRINT("!ObjectContainer - No such name of enrolled object: %s\n", _name);
	return false;
}

bool ObjectContainer::HasObject(unsigned _id)
{
	for (auto object : m_objectMap) {
		if (_id == object.second->GetId()) {
			return true;
		}
	}

	JE_DEBUG_PRINT("!ObjectContainer - No such name of enrolled object: %i\n", _id);
	return false;
}

ObjectContainer* ObjectContainer::GetCurrentContainer()
{
	return m_pSharedContainer;
}

void ObjectContainer::ClearObjectMap()
{
	for (auto obj : m_objectMap)
	{
		if (obj.second) {
			IMGUI::RemoveObjectEditor(obj.second);
			delete obj.second;
			obj.second = nullptr;
		}
	}

	IMGUI::ClearObjectEditor();
	m_objectMap.clear();
}

void ObjectContainer::EditorUpdate(const float /*_dt*/)
{
	static bool foundObject = false, showObjsList = false;

	// Main object container window
	ImGui::Begin("ObjectContainer");
	ImGui::Text("*Total Objects: %d", m_pSharedContainer->m_objectMap.size());
	if (ImGui::Button("Show Object List"))
		showObjsList = !showObjsList;

	static char ObjId[128] = "ObjectName";
	ImGui::InputText("", ObjId, IM_ARRAYSIZE(ObjId));
	
	ImGui::SameLine();
	if (ImGui::Button("Search")) {
		static Object* s_pObj = nullptr;
		s_pObj = m_pSharedContainer->GetObject(ObjId);
		s_pObj->m_showEditor = true;
	}

	ImGui::End();

	// Object list window
	if (showObjsList) {
		ImGui::Begin("ObjectList");
		for (auto Obj : m_pSharedContainer->m_objectMap)
			ImGui::Text(Obj.second->GetName().c_str());
		ImGui::End();
	}

}

JE_END

