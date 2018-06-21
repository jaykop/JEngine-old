#pragma once
#include "Macro.h"

#ifdef jeUseBuiltInAllocator

jeBegin

// If the client doesn't specify these:
//! Default number of objects per page
static const int DEFAULT_OBJECTS_PER_PAGE = 100;
//! Default Max Pages
static const int DEFAULT_MAX_PAGES = 0;

struct Node {

    template <class T>
    friend class MemoryAllocator;

private:
    Node * pNext = nullptr;
};

struct MAStats
{
    // The size of each object
    unsigned	m_objectSize = 0;
    // The number of nodes in the free list (available nodes)
    unsigned	m_freeNodes = 0;
    // The number of nodes in use (occupied nodes)
    unsigned	m_nodesInUse = 0;
    // The size of a page
    unsigned	m_pageSize = 0;
    // The nuber of pages in use (occupied pages)
    unsigned	m_pagesInUse = 0;
    // Total requests to allocate memory
    unsigned	m_allocations = 0;
    // Total requests to free memory
    unsigned	m_deallocations = 0;
    // The most nodes in use by client at one time
    unsigned	m_mostNodes = 0;
};

struct MAConfig
{
    bool        debugOn = false;
    // By-pass the functionality of the MA and use new/delete
    bool		useCPPMemoryManager = false;
    // The number of node on each page
    unsigned	nodePerPage = DEFAULT_OBJECTS_PER_PAGE;
    // Maximum number of pages the MA can allocate (0 = unlimited)
    unsigned	maxPages = DEFAULT_MAX_PAGES;
};

template <class T>
class MemoryAllocator {

    friend class Application;

    // Locked constuctor, destructor, assign operator
    MemoryAllocator& operator=(const MemoryAllocator&) = delete;
    MemoryAllocator& operator=(MemoryAllocator&&) = delete;
    MemoryAllocator(const MemoryAllocator&) = delete;
    MemoryAllocator(MemoryAllocator&&) = delete;

    enum STATUS : int {

        JE_NULL = 0x00,
        JE_UNALLOCATED = 0xaa,
        JE_ALLOCATED = 0xbb,
        JE_FREED = 0xcc

    };

public:

    MemoryAllocator();
    ~MemoryAllocator();

    Node*   Allocate();
    void    Free(T* _toReturn);

private:

    void AllocateNewPage();
    void DeallocatePage();
    void ClearPages();

    MAConfig    m_config{};
    MAStats	m_stats{};

    Node* m_freelist = nullptr;
    Node* m_pagelist = nullptr;
};

jeEnd

#include "MemoryAllocator.inl"

#endif // jeUseBuiltInAllocator