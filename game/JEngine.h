#pragma once

class JEngine
{
public:

	static bool RegisterAssets();
	static void CreateConsole();
	static void CloseConsole();
	static void SetFirstState(const char * stateName);
	static void Run(bool _imguiToggle);
};
