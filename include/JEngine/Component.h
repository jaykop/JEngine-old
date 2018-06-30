#pragma once
#include "JsonParser.h"

jeBegin

class Component {

    friend class Object;
    friend class JsonParser;
    friend class ImguiManager;
    friend class ComponentManager;

public:

    bool showEditor_;

    Object* GetOwner() const { return pOwner_; }
    virtual void Register() = 0;

protected:

    Component(Object* pOwner, bool byUser = false)
        : pOwner_(pOwner), byUser_(byUser),
        showEditor_(false), typeName_("Component") {};
    virtual	~Component() {};
    void operator=(const Component& copy) {
        byUser_ = copy.byUser_;
    };

    virtual void Load(CR_RJValue _data) = 0;
    virtual void EditorUpdate(float dt) = 0;

private:
    
    Object      *pOwner_;
    bool		byUser_;
    std::string typeName_;

    Component() = delete;;
    Component(const Component& /*copy*/) = delete;

};

#define jeDeclareStaticAllocator(x)	template <class T> friend class MemoryAllocator; static MemoryAllocator<x> allocator
#define jeDefineStaticAllocator(x)	MemoryAllocator<x> x::allocator

jeEnd
