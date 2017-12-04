#pragma once
#include "Macro.h"
#include "rapidjson/document.h"

JE_BEGIN

class ObjectContainer;
using CR_RJValue	= const rapidjson::Value&;
using RJDoc			= rapidjson::Document;
using CR_RJDoc		= const RJDoc&;

class JsonParser {

public:

	friend class State;
	friend class Application;
	friend class AssetManager;

private:

	JsonParser() {};
	~JsonParser() {};
	JsonParser(const JsonParser& /*_copy*/) = delete;
	void operator=(const JsonParser& /*_copy*/) = delete;

	static void ReadFile(const char* _dir);
	static CR_RJDoc GetDocument();

	static void LoadObjects(ObjectContainer* _pOBC);
	static void LoadComponents(CR_RJValue _data);

	static RJDoc m_document;

};

using JSON = JsonParser;

JE_END