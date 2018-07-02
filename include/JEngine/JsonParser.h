#pragma once
#include "Macro.h"
#include "document.h"

jeBegin

using CR_RJValue	= const rapidjson::Value&;
using RJDoc			= rapidjson::Document;
using CR_RJDoc		= const RJDoc&;

class JsonParser {

	friend class State;
	friend class Application;
	friend class AssetManager;

	// Locked constuctor, destructor, assign operator
	jeStaticClassDeclaration(JsonParser)

private:

	static void		ReadFile(const char* directory);
	static CR_RJDoc GetDocument();
	static void		Close();

	static void LoadObjects();
	static void LoadComponents(CR_RJValue data);

	static RJDoc document_;

};

using JSON = JsonParser;

jeEnd
