#include "ObjectFactory.h"
#include "ObjectContainer.h"
#include "ImguiManager.h"

#ifdef jeUseBuiltInAllocator
#include "MemoryAllocator.h"
#endif // jeUseBuiltInAllocator

jeBegin

//////////////////////////////////////////////////////////////////////////
// static variables
//////////////////////////////////////////////////////////////////////////
Object*			FACTORY::pLastMade_ = nullptr;
unsigned		FACTORY::registerNumber_ = 0;
bool			FACTORY::added_ = true;

#ifdef jeUseBuiltInAllocator
MemoryAllocator<Object> FACTORY::allocator;
#endif // jeUseBuiltInAllocator

void ObjectFactory::CreateObject(const char* name)
{
    // If there is ex-created and non-added object 
    // give user warning
    if (!added_)
        jeDebugPrint("!ObjectFactory - Holding not added object yet: %s\n", name);

    // unless just make new object
    else {
        added_ = false;
        ++registerNumber_;
#ifdef jeUseBuiltInAllocator
        pLastMade_ = new(allocator.Allocate()) Object(name);
#else
        pLastMade_ = new Object(name);
#endif // jeUseBuiltInAllocator
    }
}

Object* ObjectFactory::GetCreatedObject()
{
    // Return the lastly created one
    return pLastMade_;
}

void ObjectFactory::AddCreatedObject()
{
    // Register the object
	OBJECT::pContainer_->objectMap_.insert(
        ObjectMap::value_type(
            pLastMade_->name_, pLastMade_));
    IMGUI::AddObjectEditor(pLastMade_);
    pLastMade_->RegisterComponents();
    added_ = true;
}

jeEnd
