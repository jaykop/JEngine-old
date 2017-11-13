#pragma once
#include "Macro.h"
#include "rapidjson/document.h"

JE_BEGIN

class ObjectContainer;
typedef const rapidjson::Value& CR_RJValue;
typedef rapidjson::Document RJDoc;
typedef const RJDoc& CR_RJDoc;

class JsonParser {

public:

	friend class State;
	friend class Application;
	friend class AssetManager;

private:

	JsonParser() {};
	~JsonParser() {};
	JsonParser(const JsonParser& /*_copy*/) {};
	void operator=(const JsonParser& /*_copy*/) {};

	static void ReadFile(const char* _dir);
	static CR_RJDoc GetDocument();

	static void LoadObjects(ObjectContainer* _pOBC);
	static void LoadComponents(CR_RJValue _data);

	static RJDoc m_document;

};

typedef JsonParser JSON;

JE_END