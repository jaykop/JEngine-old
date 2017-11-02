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
#include "AssetManager.h"

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
	if (!SetInitData(data)) {
		JE_DEBUG_PRINT("Wrong init data!\n");
		return -1;
	}

	ASSET::RegisterComponent();
	if (APP::Initialize(data))
		APP::Update();

	APP::Close();

	DEBUG_DESTROY_CONSOLE();

	return 0;

}

bool SetInitData(APP::InitData& _data) {

	JSON::ReadFile("../src/resource/initData.json");

	const RJValue& title = JSON::GetDocument()["Title"];
	const RJValue& fullscreen = JSON::GetDocument()["Fullscreen"];
	const RJValue& width = JSON::GetDocument()["Width"];
	const RJValue& height = JSON::GetDocument()["Height"];

	if (title.IsString() && fullscreen.IsBool()
		&& width.IsInt() && height.IsInt()) {

		_data.m_title.assign(title.GetString());
		_data.m_isFullScreen = fullscreen.GetBool();
		_data.m_width = width.GetInt();
		_data.m_height = height.GetInt();
	
		return true;
	}

	else
		return false;
}