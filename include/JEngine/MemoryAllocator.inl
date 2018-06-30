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
    :config_(MAConfig())
{
    stats_.objectSize = sizeof(T);
    stats_.pageSize = stats_.objectSize * config_.nodePerPage + sizeOfPointer;
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
    static const int nextAddress = stats_.objectSize / sizeOfNode;

    if (!config_.maxPages ||			        // Unlimited pages
        stats_.pagesInUse < config_.maxPages)	// Satisfy max pages condition
    {
        // Allocate the size of memory as the total sum of the number of nodes(sizeOfNode*m_defaultNumOfNode)
        // and pointer to next page(sizeOfPointer)
        unsigned char* newPage = jeCastToUnsignedCharPointer(malloc(stats_.pageSize));

        // Convert the pages to page type
        Node* NewPageNode = jeCastToNodePointer(newPage);

        if (config_.debugOn) {
            // Set memory
            memset(newPage, JE_NULL, sizeOfPointer);
            memset(newPage + sizeOfPointer, JE_UNALLOCATED, stats_.pageSize - sizeOfPointer);
        }

        // If there were existing pages
        // Connect the current one and newly allocated page
        NewPageNode->pNext = pagelist_;

        // Move the m_pages to new page's starting point
        pagelist_ = NewPageNode;

        // Split the new page into node and connect them each other
        // by setting their pNext
        NewPageNode++;

        /*So the concept of this algorithm is set the next node of current node
        * and refesh the freelist to the newest node.
        * Then move the current node to next node and update all the next nodes of every node
        * in m_page(new page) keep doing this
        */
        for (unsigned index = 0; index < config_.nodePerPage; ++index) {

            // Connect the current node to next node
            NewPageNode->pNext = freelist_;

            // Set the freelist to the starting point of newPage 
            // which is skipping the address of pointer to next page
            freelist_ = NewPageNode;

            // Move to next node
            NewPageNode += nextAddress;
        }

        // Increase the number of availalbe ndoes and pages in use
        stats_.freeNodes_ += config_.nodePerPage;
        stats_.pagesInUse++;
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
    Node* toRemove = pagelist_;
    // Set new current page
    pagelist_ = toRemove->pNext;
    // Remove the one to remove
    free(toRemove);
}

template <class T>
void MemoryAllocator<T>::ClearPages()
{
    if (pagelist_) {
        Node* toRemove = pagelist_;

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
    if (stats_.freeNodes_ == 0)
        AllocateNewPage();

    // Get the first head from the free list 
    Node* toReturn = freelist_;
    // Set the new head of free list
    freelist_ = toReturn->pNext;
    // Decrease the number of free nodes
    stats_.freeNodes_--;
    // Increase the number of node in use and allocations
    stats_.nodesInUse_++;
    stats_.allocations_++;

    if (config_.debugOn) {
        // Set allocated pattern first
        std::memset(toReturn, JE_ALLOCATED, stats_.objectSize);
    }

    // Get the most number of nodes in used during the application is running
    unsigned total = stats_.allocations_ - stats_.deallocations_;
    if (stats_.mostNodes_ < total)
        stats_.mostNodes_ = total;

    // Return the memory;
    return toReturn;
}

template <class T>
void MemoryAllocator<T>::Free(T* toReturn)
{
    // Call its destructor
    toReturn->~T();
    // Cast to node type
    Node* newHeadFreelist = jeCastToNodePointer(toReturn);
    
    if (config_.debugOn) {
        // Set freed pattern
        std::memset(newHeadFreelist, JE_FREED, stats_.objectSize);
    }
    
    // Return the node to the free list again
    newHeadFreelist->pNext = freelist_;
    freelist_ = newHeadFreelist;
    // Increase the number of free nodes and deallocations
    stats_.freeNodes_++;
    stats_.deallocations_++;
    // Decrease the number of node in use
    stats_.nodesInUse_--;
}

jeEnd

//#endif // jeUseBuiltInAllocator
