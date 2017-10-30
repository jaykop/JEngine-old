#pragma once
#include "Macro.h"
#include "rapidjson/document.h"

JE_BEGIN

class JsonParser {

public:

	static void ReadFile(const char* _dir);
	static const rapidjson::Document& GetDocument();

	static void LoadObjects();
	static void LoadComponents(const rapidjson::Value& _data);

private:

	JsonParser() {};
	~JsonParser() {};
	JsonParser(const JsonParser& /*_copy*/) {};
	void operator=(const JsonParser& /*_copy*/) {};

	static  rapidjson::Document m_document;

};

typedef JsonParser JSON;

JE_END