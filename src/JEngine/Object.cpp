#include "Component.h"
#include "CustomComponent.h"
#include "ObjectFactory.h"
#include "ObjectContainer.h"
#include "ComponentManager.h"
#include "Telegram.h"
#include "imgui.h"
#include <string>

jeBegin

Object::Object(const char* name)
    :name_(name), active_(true), pParent_(nullptr),
    id_(ObjectFactory::registerNumber_), showEditor_(false)
{}

Object::~Object()
{
    ClearChildren();
    ClearComponents();
}

unsigned Object::GetId() const
{
    return id_;
}

void Object::RegisterComponents()
{
    for (auto component : componentMap_)
        component.second->Register();
}

const std::string& Object::GetName(void) const
{
    return name_;
}

void Object::SetName(const char* name)
{
    name_.assign(name);
}

void Object::AddChild(Object* child)
{
    // Find if there is the once
    auto found = childObjs_.find(child->GetName());

    // If there is not, add
    if (found == childObjs_.end()) {
        childObjs_.insert(
            ChildObjects::value_type(
                child->GetName(), child));

        child->SetParent(this);
    }

    // Unless...
    else
        jeDebugPrint("!Object - Existing child: %s\n", child->GetName().c_str());
}

void Object::RemoveChild(const char* name)
{
    // Find if there is the once
    auto found = childObjs_.find(name);

    // If there is, remove
    if (found != childObjs_.end()) {
        childObjs_.erase(name);		// Remove from the child list
		OBJECT::pContainer_->RemoveObject(name);	// Remove from obj manager
    }

    else
        jeDebugPrint("!Object - No such name of enrolled object: %s\n", name);
}

Object* Object::GetChild(const char* name)
{
    // Find if there is the once
    auto found = childObjs_.find(name);

    // If there is return the one
    if (found != childObjs_.end())
        return found->second;

    // Unless...
    else {
        jeDebugPrint("!Object - No such name of enrolled object: %s\n", name);
        return nullptr;
    }
}

bool Object::HasChild(const char* name)
{
    // Find if there is the once
    auto found = childObjs_.find(name);

    // If there is return the one
    if (found != childObjs_.end())
        return true;

    // Unless...
    jeDebugPrint("!Object - No such name of enrolled object: %s\n", name);
    return false;
}

void Object::SetParent(Object* pObject)
{
    pParent_ = pObject;
}

Object* Object::GetParent()
{
    return pParent_;
}

bool Object::HasParent()
{
    return pParent_ != nullptr ? true : false;
}

void Object::SetActive(bool active)
{
    active_ = active;
}

bool Object::GetActive(void) const
{
    return active_;
}

ComponentMap& Object::GetComponentMap()
{
    return componentMap_;
}

void Object::ClearComponents()
{
    // Clear all components in the list
    for (auto component : componentMap_) {

        if (component.second) {
            IMGUI::RemoveComponentEditor(component.second);
#ifdef jeUseBuiltInAllocator
            COMPONENT::RemoveComponent(component.second);
#else
            delete component.second;
            component.second = nullptr;
#endif // jeUseBuiltInAllocator
        }
    }

    IMGUI::ClearComponentEditor();
    componentMap_.clear();
}

void Object::ClearChildren()
{
    for (auto child : childObjs_)
		OBJECT::pContainer_->RemoveObject(child.second);

    childObjs_.clear();
}

void Object::AddComponent(const char* componentName)
{
    static std::string sname;
    sname = COMPONENT::KeyToTypeTranslator(componentName);
    auto found = componentMap_.find(sname);

    // Found nothing exsting component type
    // Insert new component to the list
    if (found == componentMap_.end()) {
        Component* newComponent = COMPONENT::CreateComponent(sname.c_str(), this);
        componentMap_.insert(
            ComponentMap::value_type(sname,
                newComponent));
        IMGUI::AddComponentEditor(newComponent);
    }

    else
        jeDebugPrint("!Object - Cannot add identical component again: %s\n", componentName);
}

Component* Object::GetComponent(const char* componentName)
{
    // Find if there is the one
    static std::string sname;
    sname = COMPONENT::KeyToTypeTranslator(componentName);
    auto found = componentMap_.find(sname);

    // If there is return it
    if (found != componentMap_.end())
        return found->second;

    jeDebugPrint("!Object - No such name of enrolled component: %s\n", componentName);
    return nullptr;
}

bool Object::HasComponent(const char* componentName) const
{
    // Find if there is the one
    static std::string sname;
    sname = COMPONENT::KeyToTypeTranslator(componentName);
    auto found = componentMap_.find(sname);

    // If there is return it
    if (found != componentMap_.end())
        return true;

    jeDebugPrint("!Object - No such name of enrolled component: %s\n", componentName);
    return false;

}

void Object::RemoveComponent(const char* componentName)
{
    // Find if there is the one
    auto found = componentMap_.find(componentName);

    // If there is, remove it
    if (found != componentMap_.end()) {
        IMGUI::RemoveComponentEditor(found->second);
#ifdef jeUseBuiltInAllocator
        COMPONENT::RemoveComponent(found->second);
#else
        delete found->second;
        found->second = nullptr;
#endif // jeUseBuiltInAllocator
    }

    else
        jeDebugPrint("!Object - No such name of enrolled component: %s\n", componentName);

}

bool Object::HandleMessage(Telegram& message)
{
    if (stateMachine_.pCurrentState_
        && stateMachine_.pCurrentState_->OnMessage(message))
        return true;

    if (stateMachine_.pGlobalState_
        && stateMachine_.pGlobalState_->OnMessage(message))
        return true;

    return false;
}

void Object::RevertToPreviousState()
{
    if (stateMachine_.pPreviousState_) {
        stateMachine_.pCurrentState_->Close();
        stateMachine_.pCurrentState_->Unload();
        CustomComponent* pTempState = stateMachine_.pCurrentState_;
        stateMachine_.pCurrentState_ = stateMachine_.pPreviousState_;
        stateMachine_.pPreviousState_ = pTempState;
        stateMachine_.pCurrentState_->Init();
    }

    else
        jeDebugPrint("!Object - There is not previous state.\n");
}

CustomComponent* Object::GetGlobalState() const
{
    return stateMachine_.pGlobalState_;
}

CustomComponent* Object::GetCurrentState() const
{
    return stateMachine_.pCurrentState_;
}

void Object::SetGlobalState(const char* componentName)
{
    if (!stateMachine_.pGlobalState_) {
        static std::string sname;
        sname = COMPONENT::KeyToTypeTranslator(componentName);
        auto found = componentMap_.find(sname);

        // Found same name of component,
        // then put that to global state
        if (found != componentMap_.end())
            stateMachine_.pGlobalState_ = (CustomComponent*)found->second;

        else
            jeDebugPrint("!Object - No such name of enrolled component: %s\n", componentName);
    }

    else
        jeDebugPrint("!Object - There is an allocated global state already: %s\n", componentName);
}

void Object::SetCurrentState(const char* componentName)
{
    if (!stateMachine_.pCurrentState_) {
        static std::string sname;
        sname = COMPONENT::KeyToTypeTranslator(componentName);
        auto found = componentMap_.find(sname);

        // Found same name of component,
        // then put that to current state
        if (found != componentMap_.end())
            stateMachine_.pCurrentState_ = (CustomComponent*)found->second;

        else
            jeDebugPrint("!Object - No such name of enrolled component: %s\n", componentName);
    }

    else
        jeDebugPrint("!Object - There is an allocated current state already: %s\n", componentName);
}

void Object::EditorUpdate(const float /*dt*/)
{
    // Searched object window
    if (showEditor_)
    {
        ImGui::Begin("Object");
        ImGui::Text("*Object Name: %s", name_.c_str());
        ImGui::Text("*Object Id: %d", id_);
        if (GetParent())
            ImGui::Text("*Parent Object: %s", pParent_->name_.c_str());
        else
            ImGui::Text("*Parent Object: None");

        if (!componentMap_.empty()) {
            ImGui::Text("*Component List:");
            for (auto component : componentMap_) {
                if (ImGui::Button(COMPONENT::TypeToKeyTranslator(component.first.c_str())))
                    component.second->showEditor_ = true;
            }
        }
        else
            ImGui::Text("*Component List: None");

        if (!componentMap_.empty()) {
            ImGui::Text("*Children List:");
            for (auto child : childObjs_)
                ImGui::Button(child.second->name_.c_str());
        }
        else
            ImGui::Text("*Children List: None");

        if (ImGui::Button("Close"))
            showEditor_ = false;

        ImGui::End();
    }
}

jeEnd
