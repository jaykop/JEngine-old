#pragma once
#include "Macro.h"
#include "rapidjson/document.h"

JE_BEGIN

typedef rapidjson::Value RJValue;
typedef rapidjson::Document RJDoc;

class JsonParser {

public:

	static void ReadFile(const char* _dir);
	static const RJDoc& GetDocument();

	static void LoadObjects();
	static void LoadComponents(const RJValue& _data);

private:

	JsonParser() {};
	~JsonParser() {};
	JsonParser(const JsonParser& /*_copy*/) {};
	void operator=(const JsonParser& /*_copy*/) {};

	static RJDoc m_document;

};

typedef JsonParser JSON;

JE_END