#include "Component.h"
#include "CustomComponent.h"
#include "ObjectFactory.h"
#include "ObjectContainer.h"
#include "ComponentManager.h"
#include "Telegram.h"
#include "imgui.h"
#include <string>

jeBegin

Object::Object(const char* _name)
	:m_name(_name), m_active(true), m_pParent(nullptr),
	m_id(ObjectFactory::m_registerNumber), m_showEditor(false)
{}

Object::~Object()
{
	ClearChildren();
	ClearComponents();
}

unsigned Object::GetId() const
{
	return m_id;
}

void Object::RegisterComponents()
{
	for (auto component : m_componentMap)
		component.second->Register();
}

const std::string& Object::GetName(void) const
{
	return m_name;
}

void Object::SetName(const char* _name)
{
	m_name.assign(_name);
}

void Object::AddChild(Object* _child)
{
	// Find if there is the once
	auto found = m_childObjs.find(_child->GetName());
	
	// If there is not, add
	if (found == m_childObjs.end()) {
		m_childObjs.insert(
			ChildObjects::value_type(
				_child->GetName(), _child));
		
		_child->SetParent(this);
	}

	// Unless...
	else
		jeDebugPrint("!Object - Existing child: %s\n", _child->GetName().c_str());
}

void Object::RemoveChild(const char* _name)
{
	// Find if there is the once
	auto found = m_childObjs.find(_name);

	// If there is, remove
	if (found != m_childObjs.end()) {
		m_childObjs.erase(_name);		// Remove from the child list
		CONTAINER->RemoveObject(_name);	// Remove from obj manager
	}
		
	else 
		jeDebugPrint("!Object - No such name of enrolled object: %s\n", _name);
}

Object* Object::GetChild(const char* _name)
{
	// Find if there is the once
	auto found = m_childObjs.find(_name);

	// If there is return the one
	if (found != m_childObjs.end())
		return found->second;

	// Unless...
	else {
		jeDebugPrint("!Object - No such name of enrolled object: %s\n", _name);
		return nullptr;
	}
}

bool Object::HasChild(const char* _name)
{
	// Find if there is the once
	auto found = m_childObjs.find(_name);

	// If there is return the one
	if (found != m_childObjs.end())
		return true;

	// Unless...
	jeDebugPrint("!Object - No such name of enrolled object: %s\n", _name);
	return false;
}

void Object::SetParent(Object* _pObject)
{
	m_pParent = _pObject;
}

Object* Object::GetParent()
{
	return m_pParent;
}

bool Object::HasParent()
{
	return m_pParent != nullptr ? true : false ;
}

void Object::SetActive(bool _active)
{
	m_active = _active;
}

bool Object::GetActive(void) const
{
	return m_active;
}

ComponentMap& Object::GetComponentMap()
{
	return m_componentMap;
}

void Object::ClearComponents()
{
	// Clear all components in the list
	for (auto component : m_componentMap) {

		if (component.second) {
			IMGUI::RemoveComponentEditor(component.second);
			COMPONENT::RemoveComponent(component.second);
			//delete component.second;
			//component.second = nullptr;
		}
	}

	IMGUI::ClearComponentEditor();
	m_componentMap.clear();
}

void Object::ClearChildren()
{
	for (auto child : m_childObjs)
		CONTAINER->RemoveObject(child.second->GetName().c_str());

	m_childObjs.clear();
}

void Object::AddComponent(const char* _componentName)
{
	static std::string s_name;
	s_name = COMPONENT::KeyToTypeTranslator(_componentName);
	auto found = m_componentMap.find(s_name);

	// Found nothing exsting component type
	// Insert new component to the list
	if (found == m_componentMap.end()) {
		Component* newComponent = COMPONENT::CreateComponent(s_name.c_str(), this);
		m_componentMap.insert(
			ComponentMap::value_type(s_name,
				newComponent));
		IMGUI::AddComponentEditor(newComponent);
	}

	else
		jeDebugPrint("!Object - Cannot add identical component again: %s\n", _componentName);
}

Component* Object::GetComponent(const char* _componentName)
{
	// Find if there is the one
	static std::string s_name;
	s_name = COMPONENT::KeyToTypeTranslator(_componentName);
	auto found = m_componentMap.find(s_name);

	// If there is return it
	if (found != m_componentMap.end())
		return found->second;

	jeDebugPrint("!Object - No such name of enrolled component: %s\n", _componentName);
	return nullptr;
}

bool Object::HasComponent(const char* _componentName) const
{
	// Find if there is the one
	static std::string s_name;
	s_name = COMPONENT::KeyToTypeTranslator(_componentName);
	auto found = m_componentMap.find(s_name);

	// If there is return it
	if (found != m_componentMap.end())
		return true;

	jeDebugPrint("!Object - No such name of enrolled component: %s\n", _componentName);
	return false;

}

void Object::RemoveComponent(const char* _componentName)
{
	// Find if there is the one
	auto found = m_componentMap.find(_componentName);

	// If there is, remove it
	if (found != m_componentMap.end()) {
		IMGUI::RemoveComponentEditor(found->second); 
		COMPONENT::RemoveComponent(found->second);
		//delete found->second;
		//found->second = nullptr;
	}

	else 
		jeDebugPrint("!Object - No such name of enrolled component: %s\n", _componentName);
	
}

bool Object::HandleMessage(Telegram& _message)
{
	if (m_StateMachine.m_pCurrentState
		&& m_StateMachine.m_pCurrentState->OnMessage(_message))
		return true;

	if (m_StateMachine.m_pGlobalState
		&& m_StateMachine.m_pGlobalState->OnMessage(_message))
		return true;

	return false;
}

void Object::RevertToPreviousState()
{
	if (m_StateMachine.m_pPreviousState) {
		m_StateMachine.m_pCurrentState->Close();
		m_StateMachine.m_pCurrentState->Unload();
		CustomComponent* pTempState = m_StateMachine.m_pCurrentState;
		m_StateMachine.m_pCurrentState = m_StateMachine.m_pPreviousState;
		m_StateMachine.m_pPreviousState = pTempState;
		m_StateMachine.m_pCurrentState->Init();
	}

	else
		jeDebugPrint("!Object - There is not previous state.\n");
}

CustomComponent* Object::GetGlobalState() const
{
	return m_StateMachine.m_pGlobalState;
}

CustomComponent* Object::GetCurrentState() const
{
	return m_StateMachine.m_pCurrentState;
}

void Object::SetGlobalState(const char* _componentName)
{
	if (!m_StateMachine.m_pGlobalState) {
		static std::string s_name;
		s_name = COMPONENT::KeyToTypeTranslator(_componentName);
		auto found = m_componentMap.find(s_name);

		// Found same name of component,
		// then put that to global state
		if (found != m_componentMap.end())
			m_StateMachine.m_pGlobalState = (CustomComponent*)found->second;

		else
			jeDebugPrint("!Object - No such name of enrolled component: %s\n", _componentName);
	}

	else
		jeDebugPrint("!Object - There is an allocated global state already: %s\n", _componentName);
}

void Object::SetCurrentState(const char* _componentName)
{
	if (!m_StateMachine.m_pCurrentState) {
		static std::string s_name;
		s_name = COMPONENT::KeyToTypeTranslator(_componentName);
		auto found = m_componentMap.find(s_name);

		// Found same name of component,
		// then put that to current state
		if (found != m_componentMap.end())
			m_StateMachine.m_pCurrentState = (CustomComponent*)found->second;

		else
			jeDebugPrint("!Object - No such name of enrolled component: %s\n", _componentName);
	}

	else
		jeDebugPrint("!Object - There is an allocated current state already: %s\n", _componentName);
}

void Object::EditorUpdate(const float /*_dt*/)
{
	// Searched object window
	if (m_showEditor)
	{
		ImGui::Begin("Object");
		ImGui::Text("*Object Name: %s", m_name.c_str());
		ImGui::Text("*Object Id: %d", m_id);
		if (GetParent())
			ImGui::Text("*Parent Object: %s", m_pParent->m_name.c_str());
		else
			ImGui::Text("*Parent Object: None");

		if (!m_componentMap.empty()) {
			ImGui::Text("*Component List:");
			for (auto component : m_componentMap) {
				if (ImGui::Button(COMPONENT::TypeToKeyTranslator(component.first.c_str())))
					component.second->m_showEditor = true;
			}
		}
		else
			ImGui::Text("*Component List: None");

		if (!m_componentMap.empty()) {
			ImGui::Text("*Children List:");
			for (auto child : m_childObjs)
				ImGui::Button(child.second->m_name.c_str());
		}
		else
			ImGui::Text("*Children List: None");

		if (ImGui::Button("Close"))
			m_showEditor = false;

		ImGui::End();
	}
}

jeEnd
