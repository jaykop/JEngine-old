/******************************************************************************/
/*!
\file   Platform_win32.cpp
\author Jeong Juyong
\par    email: jeykop14\@gmail.com
\date   2017/08/22(yy/mm/dd)

\description
Contains Process' main flow

*/
/******************************************************************************/

#include "Macro.h"
#include "Debug.h"
#include "JsonParser.h"
#include "Application.h"

#pragma comment(lib, "sdl2")
#pragma comment(lib, "sdl2main")

USING_NS_JE;

bool SetInitData(APP::InitData& _data);

int main(int argc, char* args[]) {

	// Make console and check memory leak
	DEBUG_LEAK_CHECKS(-1);
	DEBUG_CREATE_CONSOLE();

	// Block the warnings
	JE_UNUSED_PARAM(argc);
	JE_UNUSED_PARAM(args);

	// Check init data
	APP::InitData data;
	if (SetInitData(data)) {
		JE_DEBUG_PRINT("Wrong init data!\n");
		return -1;
	}

	if (APP::Initialize(data))
		APP::Update();

	APP::Close();

	DEBUG_DESTROY_CONSOLE();

	return 0;

}

bool SetInitData(APP::InitData& _data) {

	JSON::ReadFile("../src/resource/initData.json");

	if (JSON::GetDocument()["Title"].IsString()
		&& JSON::GetDocument()["Fullscreen"].IsBool()
		&& JSON::GetDocument()["Width"].IsInt()
		&& JSON::GetDocument()["Height"].IsInt()) {

		_data.m_title.assign(JSON::GetDocument()["Title"].GetString());
		_data.m_isFullScreen = JSON::GetDocument()["Fullscreen"].GetBool();
		_data.m_width = JSON::GetDocument()["Width"].GetInt();
		_data.m_height = JSON::GetDocument()["Height"].GetInt();
	
		return true;
	}

	else
		return false;
}