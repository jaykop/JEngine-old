#include "MemoryAllocator.h"
#include <cstdlib>
#include <cstring>

#define jeCastToNodePointer(x)			reinterpret_cast<JE::Node*>(x)
//#define jeCastToPagePointer(x)			reinterpret_cast<JE::Page*>(x)
#define jeCastToUnsignedCharPointer(x)	reinterpret_cast<unsigned char*>(x)

jeBegin

const unsigned sizeOfNode = sizeof(Node);
const unsigned sizeOfPointer = sizeof(void*);
const unsigned sizeOfUnsignedChar = sizeof(unsigned char);

p_unitMemory	MEMORY::m_pages = nullptr;
p_unitMemory	MEMORY::m_freelist = nullptr;
p_unitMemory	MEMORY::m_firstPage = nullptr;
bool			MEMORY::m_useDefaultkeyword = false;
unsigned		MEMORY::m_freeNodes = 0, 
				MEMORY::m_objectInUse = 0,
				MEMORY::m_allocations = 0,
				MEMORY::m_deallocations = 0,
				MEMORY::m_mostObjects = 0;

void MemoryAllocator::AllocateNewPage()
{
	unsigned availablePageSize = sizeOfNode * m_defaultNumOfNodes;

	// Allocate the size of memory as the total sum of the number of nodes(sizeOfNode*m_defaultNumOfNode)
	// and pointer to next page(sizeOfPointer)
	unsigned char* newPage = jeCastToUnsignedCharPointer(malloc(availablePageSize + sizeOfPointer));

	// If there were existing pages
	if (m_pages) {

		// Convert the pages to page type
		Node* currentPage = jeCastToNodePointer(m_pages);
		// Connect the current one and newly allocated page
		currentPage->pNext = jeCastToNodePointer(newPage);
	}
	// Otherwise, just connect the new page

	// Move the m_pages to new page's starting point
	m_pages = newPage;
	memset(m_pages, JE_NULL, sizeOfPointer);
	memset(m_pages + sizeOfPointer, JE_UNALLOCATED, availablePageSize);

	// Split the new page into node and connect them each other
	// by setting their pNext
	Node* toMove = jeCastToNodePointer(m_pages + sizeOfPointer);
	/*for (unsigned index = 0; index < ;) {
		toMove->pNext = ;
	}*/

	// If we didn't get the address of the first page than
	// get it - but just one time
	if (!m_firstPage) m_firstPage = m_pages;

	// Set the freelist to the starting point of newPage 
	// which is skipping the address of pointer to next page
	m_freelist = m_pages;

	m_freeNodes += m_defaultNumOfNodes;

}

void MemoryAllocator::DeallocatePage()
{
	// Remove current page

	Node* toMove = jeCastToNodePointer(m_firstPage);
	Node* newCurrent = toMove;

	// Get the last page
	while (toMove) {

		// Save the page right before the one to remove
		newCurrent = toMove;
		// Keep moving to next page
		toMove = toMove->pNext;
	}

	// Set new current page
	m_pages = jeCastToUnsignedCharPointer(newCurrent);
	// Remove the one to remove
	free(toMove);
}

void MemoryAllocator::ClearPages()
{
	Node* toRemove = jeCastToNodePointer(m_firstPage);
	Node* next = toRemove->pNext;

	while (toRemove) {
		free(toRemove);		// Remove current one
		toRemove = next;	// Get next one to current one

		if (toRemove)
			next = toRemove->pNext;		// Get new next one
	}
}

Node* MemoryAllocator::Allocate()
{
	// If there not enough memory to return,
	// make new page
	if (m_freeNodes == 0)
		AllocateNewPage();

	// Get the first head from the free list 
	Node* toReturn = jeCastToNodePointer(m_freelist);
	// Set the new head of free list
	m_freelist = jeCastToUnsignedCharPointer(toReturn->pNext);
	// Decrease the number of free nodes
	m_freeNodes--;
	// Increase the number of object in use and allocations
	m_objectInUse++;
	m_allocations++;
	// Set allocated pattern first
	std::memset(toReturn, JE_ALLOCATED, sizeOfNode);

	// Get the most number of objects in used during the application is running
	unsigned total = m_allocations - m_deallocations;
	if (m_mostObjects < total)
		m_mostObjects = total;

	// Return the memory;
	return toReturn;
}

void MemoryAllocator::Free(Node* _toReturn)
{
	// Set freed pattern
	std::memset(_toReturn, JE_FREED, sizeOfNode);
	// Return the node to the free list again
	_toReturn->pNext = jeCastToNodePointer(m_freelist);
	m_freelist = jeCastToUnsignedCharPointer(_toReturn);
	// Increase the number of free nodes and deallocations
	m_freeNodes++;
	m_deallocations++;
	// Decrease the number of object in use
	m_objectInUse--;
}

jeEnd
