//#ifdef jeUseBuiltInAllocator

#include <cstdlib>
#include <cstring>

#define jeCastToNodePointer(x)		reinterpret_cast<JE::Node*>(x)
#define jeCastToUnsignedCharPointer(x)	reinterpret_cast<unsigned char*>(x)

jeBegin

const unsigned sizeOfPointer = sizeof(void*);
const unsigned sizeOfNode = sizeof(Node);

template <class T>
MemoryAllocator<T>::MemoryAllocator()
    :m_config(MAConfig())
{
    m_stats.m_objectSize = sizeof(T);
    m_stats.m_pageSize = m_stats.m_objectSize * m_config.nodePerPage + sizeOfPointer;
    AllocateNewPage();
}

template <class T>
MemoryAllocator<T>::~MemoryAllocator()
{
    ClearPages();
}

template <class T>
void MemoryAllocator<T>::AllocateNewPage()
{
    static const int nextAddress = m_stats.m_objectSize / sizeOfNode;

    if (!m_config.maxPages ||			        // Unlimited pages
        m_stats.m_pagesInUse < m_config.maxPages)	// Satisfy max pages condition
    {
        // Allocate the size of memory as the total sum of the number of nodes(sizeOfNode*m_defaultNumOfNode)
        // and pointer to next page(sizeOfPointer)
        unsigned char* newPage = jeCastToUnsignedCharPointer(malloc(m_stats.m_pageSize));

        // Convert the pages to page type
        Node* NewPageNode = jeCastToNodePointer(newPage);

        if (m_config.debugOn) {
            // Set memory
            memset(newPage, JE_NULL, sizeOfPointer);
            memset(newPage + sizeOfPointer, JE_UNALLOCATED, m_stats.m_pageSize - sizeOfPointer);
        }

        // If there were existing pages
        // Connect the current one and newly allocated page
        NewPageNode->pNext = m_pagelist;

        // Move the m_pages to new page's starting point
        m_pagelist = NewPageNode;

        // Split the new page into node and connect them each other
        // by setting their pNext
        NewPageNode++;

        /*So the concept of this algorithm is set the next node of current node
        * and refesh the freelist to the newest node.
        * Then move the current node to next node and update all the next nodes of every node
        * in m_page(new page) keep doing this
        */
        for (unsigned index = 0; index < m_config.nodePerPage; ++index) {

            // Connect the current node to next node
            NewPageNode->pNext = m_freelist;

            // Set the freelist to the starting point of newPage 
            // which is skipping the address of pointer to next page
            m_freelist = NewPageNode;

            // Move to next node
            NewPageNode += nextAddress;
        }

        // Increase the number of availalbe ndoes and pages in use
        m_stats.m_freeNodes += m_config.nodePerPage;
        m_stats.m_pagesInUse++;
    }

}

template <class T>
void MemoryAllocator<T>::DeallocatePage()
{
    /* This function will be operated under the condition
    * that current page is empty - all of nodes in this page are availble
    * so no reason to maintain this page
    */

    // Remove current page
    Node* toRemove = m_pagelist;
    // Set new current page
    m_pagelist = toRemove->pNext;
    // Remove the one to remove
    free(toRemove);
}

template <class T>
void MemoryAllocator<T>::ClearPages()
{
    if (m_pagelist) {
        Node* toRemove = m_pagelist;

        while (toRemove) {

            // Save next page of current one
            Node* next = toRemove->pNext;
            // Remove current one
            free(toRemove);
            // Get new next one
            toRemove = next;
        }
    }
}

template <class T>
Node* MemoryAllocator<T>::Allocate()
{
    // If there not enough memory to return,
    // make new page
    if (m_stats.m_freeNodes == 0)
        AllocateNewPage();

    // Get the first head from the free list 
    Node* toReturn = m_freelist;
    // Set the new head of free list
    m_freelist = toReturn->pNext;
    // Decrease the number of free nodes
    m_stats.m_freeNodes--;
    // Increase the number of node in use and allocations
    m_stats.m_nodesInUse++;
    m_stats.m_allocations++;

    if (m_config.debugOn) {
        // Set allocated pattern first
        std::memset(toReturn, JE_ALLOCATED, m_stats.m_objectSize);
    }

    // Get the most number of nodes in used during the application is running
    unsigned total = m_stats.m_allocations - m_stats.m_deallocations;
    if (m_stats.m_mostNodes < total)
        m_stats.m_mostNodes = total;

    // Return the memory;
    return toReturn;
}

template <class T>
void MemoryAllocator<T>::Free(T* _toReturn)
{
    // Call its destructor
    _toReturn->~T();
    // Cast to node type
    Node* newHeadFreelist = jeCastToNodePointer(_toReturn);
    
    if (m_config.debugOn) {
        // Set freed pattern
        std::memset(newHeadFreelist, JE_FREED, m_stats.m_objectSize);
    }
    
    // Return the node to the free list again
    newHeadFreelist->pNext = m_freelist;
    m_freelist = newHeadFreelist;
    // Increase the number of free nodes and deallocations
    m_stats.m_freeNodes++;
    m_stats.m_deallocations++;
    // Decrease the number of node in use
    m_stats.m_nodesInUse--;
}

jeEnd

//#endif // jeUseBuiltInAllocator
