#pragma once
#include "Macro.h"

jeBegin

#ifdef jeUseBuiltInAllocator
template <class T>
class MemoryAllocator;
#endif // jeUseBuiltInAllocator

class ObjectFactory {

    friend class Object;
    friend class ObjectContainer;

    // Locked constuctor, destructor, assign operator
    jeStaticClassDeclaration(ObjectFactory)

#ifdef jeUseBuiltInAllocator
    static MemoryAllocator<Object> allocator;
#endif // jeUseBuiltInAllocator

public:

    static void	    CreateObject(const char* name);
    static Object*  GetCreatedObject();
    static void	    AddCreatedObject();

private:

    static bool	    added_;
    static unsigned registerNumber_;
    static Object*  pLastMade_;

};

using FACTORY = ObjectFactory;

jeEnd
