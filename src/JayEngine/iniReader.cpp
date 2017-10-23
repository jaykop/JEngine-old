/******************************************************************************/
/*!
\file   iniReader.cpp
\author Jeong Juyong
\par    email: jeykop14\@gmail.com
\date   2016/07/28(yy/mm/dd)

\description
Contains iniReader member function

All content (C) 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include <iostream>
#include <Windows.h>
#include "iniReader.h"

JE_BEGIN

/******************************************************************************/
/*!
\brief - iniReader Constructor
\param fileName - fileName to write
*/
/******************************************************************************/
iniReader::iniReader(const char* fileName)
{
	// Get file name
	memset(m_fileName, 0x00, 255);
	memcpy(m_fileName, fileName, strlen(fileName));
}

/******************************************************************************/
/*!
\brief - iniReader Destructor
*/
/******************************************************************************/
iniReader::~iniReader()
{
	// Delete temp dynamic new
	delete [] m_stringResult;
}

/******************************************************************************/
/*!
\brief - Read int to ini file
\param section
\param key
\param DefaultInt - int to read
\return iResult
*/
/******************************************************************************/
int iniReader::ReadInt(char *section, char* key)
{
	int DefaultInt = 0;
	int iResult = GetPrivateProfileInt(section, key, DefaultInt, m_fileName);
	return iResult;
}

/******************************************************************************/
/*!
\brief - Read float to ini file
\param section
\param key
\param DefaultFloat - float to read
\return fltResult
*/
/******************************************************************************/
float iniReader::ReadFloat(char *section, char* key)
{
	float DefaultFloat = 0.f;
	char Result[255];
	char Default[255];
	wsprintf(Default, "%f", DefaultFloat);
	GetPrivateProfileString(section, key, Default, Result, 255, m_fileName);
	float fltResult = static_cast<float>(atof(Result));
	return fltResult;
}

/******************************************************************************/
/*!
\brief - Read bool to ini file
\param section
\param key
\param DefaultBoolean - bool to read
\return bolResult
*/
/******************************************************************************/
bool iniReader::ReadBool(char *section, char* key)
{
	bool DefaultBoolean = false;
	char Result[255];
	char Default[255];
	wsprintf(Default, "%s", DefaultBoolean ? "True" : "False");
	GetPrivateProfileString(section, key, Default, Result, 255, m_fileName);
	bool bolResult = (strcmp(Result, "True") == 0 ||
		strcmp(Result, "true") == 0) ? true : false;
	return bolResult;
}

/******************************************************************************/
/*!
\brief - Read string to ini file
\param section
\param key
\param DefaultString - string to read
\return m_stringResult
*/
/******************************************************************************/
char* iniReader::ReadString(char *section, char* key)
{
	const char* DefaultString = "";
	m_stringResult = new char[255];
	memset(m_stringResult, 0x00, 255);
	 GetPrivateProfileString(section, key, 
		 DefaultString, m_stringResult, 255, m_fileName);
	 return m_stringResult;
}

///******************************************************************************/
///*!
//\brief - Read vec3 to ini file
//\param section
//\param key
//\param DefaultBoolean - vec3 to read
//*/
///******************************************************************************/
//vec3 iniReader::ReadVec3(char *section, char* key)
//{
//	vec3 vec3Result;
//	const vec3 DefaultVec3;
//	char Result[2][255];
//	char Default[255];
//
//	wsprintf(Default, "%f", DefaultVec3);
//	GetPrivateProfileString(section, key, Default, Result[0], 255, m_fileName);
//	for (int index = 0, fill = 0; index < sizeof(Result[0]) || fill < 3; ++index)
//	{
//		char pt[2] = { Result[0][index], '\0' };
//
//		if (!strcmp(pt, " "))
//		{
//			strcpy_s(Result[1], (Result[0] + index + 1));
//			if (fill == 0) vec3Result.x = static_cast<float>(atof(Result[1]));
//			else if (fill == 1) vec3Result.y = static_cast<float>(atof(Result[1]));
//			else if (fill == 2) vec3Result.z = static_cast<float>(atof(Result[1]));
//			++fill;
//		}
//	}
//
//	return vec3Result;
//}
//
///******************************************************************************/
///*!
//\brief - Read vec4 to ini file
//\param section
//\param key
//\param DefaultBoolean - vec4 to read
//*/
///******************************************************************************/
//vec4 iniReader::ReadVec4(char *section, char* key)
//{
//	vec4 vec4Result;
//	const vec4 DefaultVec4;
//	char Result[2][255];
//	char Default[255];
//
//	wsprintf(Default, "%f", DefaultVec4);
//	GetPrivateProfileString(section, key, Default, Result[0], 255, m_fileName);
//	
//	for (int index = 0, fill = 0; index < sizeof(Result[0]) && fill < 4; ++index)
//	{
//		char pt[2] = { Result[0][index], '\0' };
//
//		if (!strcmp(pt, " "))
//		{
//			strcpy_s(Result[1], (Result[0] + index + 1));
//			if (fill == 0)
//				vec4Result.x = static_cast<float>(atof(Result[1]));
//			else if (fill == 1) 
//				vec4Result.y = static_cast<float>(atof(Result[1]));
//			else if (fill == 2) 
//				vec4Result.z = static_cast<float>(atof(Result[1]));
//			else if (fill == 3) 
//				vec4Result.w = static_cast<float>(atof(Result[1])); 
//			++fill;
//		}
//	}
//
//	return vec4Result;
//}

JE_END
