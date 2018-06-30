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

void ObjectContainer::RemoveObject(const char* name)
{
    // Find the one to remove
    auto toRemove = objectMap_.find(name);

    // If found the one
    if (toRemove != objectMap_.end()) {
        IMGUI::RemoveObjectEditor(toRemove->second);
#ifdef jeUseBuiltInAllocator
        FACTORY::allocator.Free(toRemove->second);
#else
        delete toRemove->second;
        toRemove->second = nullptr;
#endif // jeUseBuiltInAllocator
        objectMap_.erase(toRemove);
    }

    else
        jeDebugPrint("!ObjectContainer - No such name of enrolled object: %s\n", name);
}

void ObjectContainer::RemoveObject(Object* pObject)
{
    bool notFound = true;
    for (auto object = objectMap_.begin();
        object != objectMap_.end(); ++object) {

        if (pObject == object->second) {
            notFound = false;
            IMGUI::RemoveObjectEditor(object->second);
#ifdef jeUseBuiltInAllocator
            FACTORY::allocator.Free(object->second);
#else
            delete object->second;
            object->second = nullptr;
#endif // jeUseBuiltInAllocator
            objectMap_.erase(object);

            break;
        }
    }

    if (notFound)
        jeDebugPrint("!ObjectContainer - No such name of enrolled object: %s\n", pObject->GetName().c_str());
}

void ObjectContainer::RemoveObject(unsigned id)
{
    bool notFound = true;
    for (auto object = objectMap_.begin();
        object != objectMap_.end(); ++object) {

        Object* theOne = object->second;
        if (id == theOne->GetId()) {
            notFound = false;
            IMGUI::RemoveObjectEditor(theOne);
#ifdef jeUseBuiltInAllocator
            FACTORY::allocator.Free(theOne);
#else
            delete theOne;
            theOne = nullptr;
#endif // jeUseBuiltInAllocator
            objectMap_.erase(object);

            break;
        }
    }

    if (notFound)
        jeDebugPrint("!ObjectContainer - No such name of enrolled object: %i\n", id);
}

Object* ObjectContainer::GetObject(const char *name)
{
    // Find the one to remove
    auto found = objectMap_.find(name);

    // If found the one
    if (found != objectMap_.end())
        return found->second;

    jeDebugPrint("!ObjectContainer - No such name of enrolled object: %s\n", name);
    return nullptr;

}

Object* ObjectContainer::GetObject(unsigned id)
{
    for (auto object : objectMap_) {
        if (id == object.second->GetId()) {
            return object.second;
        }
    }

    jeDebugPrint("!ObjectContainer - No such name of enrolled object: %i\n", id);
    return nullptr;
}

ObjectMap& ObjectContainer::GetObjectMap()
{
    return objectMap_;
}

bool ObjectContainer::HasObject(const char* name)
{
    // Find the one to remove
    auto found = objectMap_.find(name);

    // If found the one
    if (found != objectMap_.end())
        return true;

    jeDebugPrint("!ObjectContainer - No such name of enrolled object: %s\n", name);
    return false;
}

bool ObjectContainer::HasObject(unsigned id)
{
    for (auto object : objectMap_) {
        if (id == object.second->GetId()) {
            return true;
        }
    }

    jeDebugPrint("!ObjectContainer - No such name of enrolled object: %i\n", id);
    return false;
}

void ObjectContainer::ClearObjectMap()
{
    for (auto obj : objectMap_)
    {
        if (obj.second && !obj.second->pParent_) {
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
    objectMap_.clear();
}

void ObjectContainer::EditorUpdate(const float /*dt*/)
{
    static bool foundObject = false, showObjsList = false;

    // Main object container window
    ImGui::Begin("ObjectContainer");
    ImGui::Text("*Total Objects: %d", pContainer_->objectMap_.size());
    if (ImGui::Button("Show Object List"))
        showObjsList = !showObjsList;

    static char ObjId[128] = "ObjectName";
    ImGui::InputText("", ObjId, IM_ARRAYSIZE(ObjId));

    ImGui::SameLine();
    if (ImGui::Button("Search")) {
        static Object* spObject = nullptr;
        spObject = pContainer_->GetObject(ObjId);
        spObject->showEditor_ = true;
    }

    ImGui::End();

    // Object list window
    if (showObjsList) {
        ImGui::Begin("ObjectList");
        for (auto Obj : pContainer_->objectMap_)
            ImGui::Text(Obj.second->GetName().c_str());
        ImGui::End();
    }

}

jeEnd

