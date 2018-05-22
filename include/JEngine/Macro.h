/******************************************************************************/
/*!
\file   Macro.h
\author Jeong Juyong
\par    email: jeykop14\@gmail.com
\date   2017/08/23(yy/mm/dd)

\description
Contains Engine's generic macros

*/
/******************************************************************************/

#pragma once

// Generic Engine Macro
#ifdef __cplusplus
#define jeBegin	namespace JE {
#define jeEnd	}
#define jeUsingNamespace	using namespace JE
#define jeNamespace		::JE
#else
#define jeBegin 
#define jeEnd	 
#define jeUsingNamespace 
#define jeNamespace 
#endif //__cplusplus

// Debug printing
#define jeDoPrint
#if defined(_DEBUG) && defined(jeDoPrint)
#include <cstdio>
#define jeDebugPrint(...) printf(__VA_ARGS__)
#else
#define jeDebugPrint(...)	jeUnusedParam(__VA_ARGS__)
#endif // defined(_DEBUG) && defined(JE_DO_PRINT)

// Define NULL pointer
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void*)0)
#endif // __cplusplus
#endif// NULL

// Disable asserttion
#define NDEBUG
#include <assert.h>

// Set variable as unreferenced
#define jeUnusedParam(unused)    (void)unused

// Delete macro
#define jeSafeDelete(p)			do { if(p) { delete(p);  (p) = nullptr;}} while(0)
#define jeSafeDeleteArray(p)	do { if(p) { delete[] (p); (p) = nullptr;}} while(0)

// Simplify static setting
#define jeStaticClassDeclaration(c) c() = delete; ~c() = delete; c& operator=(const c&) = delete; c& operator=(c&&) = delete; c(const c&) = delete; c(c&&) = delete;