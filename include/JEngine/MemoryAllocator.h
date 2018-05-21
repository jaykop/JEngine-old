#pragma once
#include "Macro.h"

jeBegin

using p_unitMemory = unsigned char *;

struct Node{

	Node* pNext = nullptr;

};

class MemoryAllocator {
	
	friend struct Page;
	friend class Application;

	// Locked constuctor, destructor, assign operator
	jeStaticClassDeclaration(MemoryAllocator)

	enum STATUS : int {

		JE_NULL = 0x00,
		JE_UNALLOCATED = 0xaa,
		JE_ALLOCATED = 0xbb,
		JE_FREED = 0xcc

	};

public:

private:

	static void AllocateNewPage();
	static void DeallocatePage();
	static void ClearPages();

	static Node*	Allocate();
	static void		Free(Node* _toReturn);

	static bool					m_useDefaultkeyword;
	static constexpr unsigned	m_defaultNumOfNodes = 100;
	static unsigned				m_freeNodes, m_objectInUse, m_allocations, m_deallocations, m_mostObjects;

	static p_unitMemory m_freelist;
	static p_unitMemory m_pages;
	static p_unitMemory m_firstPage;
};

using MEMORY = MemoryAllocator;

//struct Page {
//	Page* pNext = nullptr;
//};


jeEnd