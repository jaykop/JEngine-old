#pragma once
#include "Macro.h"
#include "document.h"

JE_BEGIN

using CR_RJValue	= const rapidjson::Value&;
using RJDoc			= rapidjson::Document;
using CR_RJDoc		= const RJDoc&;

class JsonParser {

public:

	friend class State;
	friend class Application;
	friend class AssetManager;

private:

	JsonParser() = delete;
	~JsonParser() = delete;
	JsonParser(const JsonParser& /*_copy*/) = delete;
	void operator=(const JsonParser& /*_copy*/) = delete;

	static void		ReadFile(const char* _dir);
	static CR_RJDoc GetDocument();
	static void		Close();

	static void LoadObjects();
	static void LoadComponents(CR_RJValue _data);

	static RJDoc m_document;

};

using JSON = JsonParser;

JE_END