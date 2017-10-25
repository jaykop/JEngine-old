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
#define JE_BEGIN	namespace JEngine {
#define JE_END	}
#define USING_NS_JE	using namespace JEngine
#define NS_JE		::JEngine
#else
#define NS_BEGIN 
#define NE_END	 
#define USING_NS 
#define NS_JE 
#endif //__cplusplus

// Debug printing
#define JE_DO_PRINT
#if defined(_DEBUG) && defined(JE_DO_PRINT)
#include <cstdio>
#define JE_DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
#define JE_DEBUG_PRINT(...)	JE_UNUSED_PARAM(__VA_ARGS__)
#endif // defined(_DEBUG) && defined(JE_DO_PRINT)

// Activate 3d features
//#define JE_SUPPORT_3D

// Activate imgui features
// #define JE_SUPPORT_IMGUI

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
#define JE_UNUSED_PARAM(unused)    (void)unused

// Delete macro
#define JE_SAFE_DELETE(p)			do { if(p) { delete(p);  (p) = nullptr;}} while(0)
#define JE_SAFE_DELETE_ARRAY(p)		do { if(p) { delete[] (p); (p) = nullptr;}} while(0)

// And the other...?