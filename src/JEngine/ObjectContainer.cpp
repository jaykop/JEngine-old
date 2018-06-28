#include "Object.h"
#include "ObjectContainer.h"
#include "imgui.h"
#include "ImguiManager.h"
#include "ObjectFactory.h"
#include "MemoryAllocator.h"

jeBegin

ObjectContainer* OBJECT::pContainer_ = nullptr;

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
#ifdef jeUseBuiltInAllocator
        FACTORY::allocator.Free(toRemove->second);
#else
        delete toRemove->second;
        toRemove->second = nullptr;
#endif // jeUseBuiltInAllocator
        m_objectMap.erase(toRemove);
    }

    else
        jeDebugPrint("!ObjectContainer - No such name of enrolled object: %s\n", _name);
}

void ObjectContainer::RemoveObject(Object* _pObj)
{
    bool notFound = true;
    for (auto object = m_objectMap.begin();
        object != m_objectMap.end(); ++object) {

        if (_pObj == object->second) {
            notFound = false;
            IMGUI::RemoveObjectEditor(object->second);
#ifdef jeUseBuiltInAllocator
            FACTORY::allocator.Free(object->second);
#else
            delete object->second;
            object->second = nullptr;
#endif // jeUseBuiltInAllocator
            m_objectMap.erase(object);

            break;
        }
    }

    if (notFound)
        jeDebugPrint("!ObjectContainer - No such name of enrolled object: %s\n", _pObj->GetName().c_str());
}

void ObjectContainer::RemoveObject(unsigned _id)
{
    bool notFound = true;
    for (auto object = m_objectMap.begin();
        object != m_objectMap.end(); ++object) {

        Object* theOne = object->second;
        if (_id == theOne->GetId()) {
            notFound = false;
            IMGUI::RemoveObjectEditor(theOne);
#ifdef jeUseBuiltInAllocator
            FACTORY::allocator.Free(theOne);
#else
            delete theOne;
            theOne = nullptr;
#endif // jeUseBuiltInAllocator
            m_objectMap.erase(object);

            break;
        }
    }

    if (notFound)
        jeDebugPrint("!ObjectContainer - No such name of enrolled object: %i\n", _id);
}

Object* ObjectContainer::GetObject(const char *_name)
{
    // Find the one to remove
    auto found = m_objectMap.find(_name);

    // If found the one
    if (found != m_objectMap.end())
        return found->second;

    jeDebugPrint("!ObjectContainer - No such name of enrolled object: %s\n", _name);
    return nullptr;

}

Object* ObjectContainer::GetObject(unsigned _id)
{
    for (auto object : m_objectMap) {
        if (_id == object.second->GetId()) {
            return object.second;
        }
    }

    jeDebugPrint("!ObjectContainer - No such name of enrolled object: %i\n", _id);
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

    jeDebugPrint("!ObjectContainer - No such name of enrolled object: %s\n", _name);
    return false;
}

bool ObjectContainer::HasObject(unsigned _id)
{
    for (auto object : m_objectMap) {
        if (_id == object.second->GetId()) {
            return true;
        }
    }

    jeDebugPrint("!ObjectContainer - No such name of enrolled object: %i\n", _id);
    return false;
}

void ObjectContainer::ClearObjectMap()
{
    for (auto obj : m_objectMap)
    {
        if (obj.second && !obj.second->m_pParent) {
            IMGUI::RemoveObjectEditor(obj.second);
#ifdef jeUseBuiltInAllocator
            FACTORY::allocator.Free(obj.second);
#else
            delete obj.second;
            obj.second = nullptr;
#endif // jeUseBuiltInAllocator
        }
    }

    IMGUI::ClearObjectEditor();
    m_objectMap.clear();
}

void ObjectContainer::EditorUpdate(const float /*dt*/)
{
    static bool foundObject = false, showObjsList = false;

    // Main object container window
    ImGui::Begin("ObjectContainer");
    ImGui::Text("*Total Objects: %d", pContainer_->m_objectMap.size());
    if (ImGui::Button("Show Object List"))
        showObjsList = !showObjsList;

    static char ObjId[128] = "ObjectName";
    ImGui::InputText("", ObjId, IM_ARRAYSIZE(ObjId));

    ImGui::SameLine();
    if (ImGui::Button("Search")) {
        static Object* s_pObj = nullptr;
        s_pObj = pContainer_->GetObject(ObjId);
        s_pObj->m_showEditor = true;
    }

    ImGui::End();

    // Object list window
    if (showObjsList) {
        ImGui::Begin("ObjectList");
        for (auto Obj : pContainer_->m_objectMap)
            ImGui::Text(Obj.second->GetName().c_str());
        ImGui::End();
    }

}

jeEnd

