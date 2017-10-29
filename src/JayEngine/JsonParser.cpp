#include "JsonParser.h"
#include "FileReadStream.h"
#include "stringbuffer.h"

JE_BEGIN

rapidjson::Document JsonParser::m_document;

void JsonParser::ReadFile(const char * _dir)
{
	m_document.Clear();
	FILE* pFile;
	fopen_s(&pFile, _dir, "rb");
	char buffer[65536];
	rapidjson::FileReadStream read(pFile, buffer, sizeof(buffer));
	m_document.ParseStream<0, rapidjson::UTF8<>, rapidjson::FileReadStream>(read);
}

const rapidjson::Document & JsonParser::GetDocument()
{
	return m_document;
}

JE_END

