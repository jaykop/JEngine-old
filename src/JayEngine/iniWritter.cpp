/******************************************************************************/
/*!
\file   iniWritter.cpp
\author Jeong Juyong
\par    email: jeykop14\@gmail.com
\date   2016/07/28(yy/mm/dd)

\description
Contains iniWritter member function

All content (C) 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include <cstdio>
#include <iostream>
#include <atlstr.h>
#include "iniWritter.h"

NS_JE_BEGIN

/******************************************************************************/
/*!
\brief - iniWritter Constructor
\param fileName - fileName to write
*/
/******************************************************************************/
iniWritter::iniWritter(const char* fileName)
{
	// Get file name
	memset(m_fileName, 0x00, 255);
	memcpy(m_fileName, fileName, strlen(fileName));
}

/******************************************************************************/
/*!
\brief - Write int to ini file
\param section
\param key
\param DefaultInt - int to write
*/
/******************************************************************************/
void iniWritter::WriteInt(char *section, char* key, int DefaultInt)
{
	char value[255];
	wsprintf(value, "%d", DefaultInt);
	WritePrivateProfileString(section, key, value, m_fileName);
}

/******************************************************************************/
/*!
\brief - Write float to ini file
\param section
\param key
\param DefaultFloat - float to write
*/
/******************************************************************************/
void iniWritter::WriteFloat(char *section, char* key, float DefaultFloat)
{
	char value[255];
	_stprintf_s(value, "%f", DefaultFloat);
	WritePrivateProfileString(section, key, value, m_fileName);
}

/******************************************************************************/
/*!
\brief - Write boolean to ini file
\param section
\param key
\param DefaultBoolean - boolean to write
*/
/******************************************************************************/
void iniWritter::WriteBool(char *section, char* key, bool DefaultBoolean)
{
	char value[255];
	wsprintf(value, "%s", DefaultBoolean ? "True" : "False");
	WritePrivateProfileString(section, key, value, m_fileName);
}

/******************************************************************************/
/*!
\brief - Write string to ini file
\param section
\param key
\param DefaultString - string to write
*/
/******************************************************************************/
void iniWritter::WriteString(char *section, char* key, const char* DefaultString)
{
	WritePrivateProfileString(section, key, DefaultString, m_fileName);
}
//
///******************************************************************************/
///*!
//\brief - Write vec3 to ini file
//\param section
//\param key
//\param DefaultBoolean - vec3 to write
//*/
///******************************************************************************/
//void iniWritter::WriteVec3(char *section, char* key, const vec3& DefaultVec3)
//{
//	char value[255];
//	_stprintf_s(value, "[ %f, %f, %f]", DefaultVec3.x, DefaultVec3.y, DefaultVec3.z);
//	WritePrivateProfileString(section, key, value, m_fileName);
//}
//
///******************************************************************************/
///*!
//\brief - Write vec4 to ini file
//\param section
//\param key
//\param DefaultBoolean - vec4 to write
//*/
///******************************************************************************/
//void iniWritter::WriteVec4(char *section, char* key, const vec4& DefaultVec4)
//{
//	char value[255];
//	_stprintf_s(value, "[ %f, %f, %f, %f ]", DefaultVec4.x, DefaultVec4.y, DefaultVec4.z, DefaultVec4.w);
//	WritePrivateProfileString(section, key, value, m_fileName);
//}

NS_JE_END