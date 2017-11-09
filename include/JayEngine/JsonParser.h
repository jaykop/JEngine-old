#pragma once
#include "Macro.h"
#include "rapidjson/document.h"

JE_BEGIN

class ObjectContainer;
typedef rapidjson::Value RJValue;
typedef const RJValue& CR_RJValue;
typedef rapidjson::Document RJDoc;

class JsonParser {

public:

	friend class State;
	friend class AssetManager;

private:

	JsonParser() {};
	~JsonParser() {};
	JsonParser(const JsonParser& /*_copy*/) {};
	void operator=(const JsonParser& /*_copy*/) {};

	static void ReadFile(const char* _dir);
	static const RJDoc& GetDocument();

	static void LoadObjects(ObjectContainer* _pOBC);
	static void LoadComponents(CR_RJValue _data);

	static RJDoc m_document;

};

typedef JsonParser JSON;

JE_END