#include "Object.h"
#include "Component.h"
#include "JsonParser.h"
#include "ObjectFactory.h"
#include "ObjectContainer.h"
#include "stringbuffer.h"
#include "FileReadStream.h"

JE_BEGIN

RJDoc		JsonParser::m_document;

void JsonParser::ReadFile(const char * _dir)
{
	m_document.Clear();
	FILE* pFile;
	fopen_s(&pFile, _dir, "rb");
	char buffer[65536];
	rapidjson::FileReadStream read(pFile, buffer, sizeof(buffer));
	m_document.ParseStream<0, rapidjson::UTF8<>, rapidjson::FileReadStream>(read);
}

CR_RJDoc JsonParser::GetDocument()
{
	return m_document;
}

void JsonParser::LoadObjects(ObjectContainer* _pOBC)
{
	CR_RJValue object = m_document["Object"];

	for (rapidjson::SizeType i = 0; i < object.Size(); ++i) {

		// Check either if object's name
		if (object[i]["Name"].IsString()) {
			FACTORY::CreateObject(object[i]["Name"].GetString());

			// Check either if object has any component
			if (object[i].HasMember("Component")) {
				CR_RJValue component = object[i]["Component"];

				// Check either if components have correct type
				for (rapidjson::SizeType j = 0; j < component.Size(); ++j) {

					if (component[j].HasMember("Type"))
						LoadComponents(component[j]);
					else
						JE_DEBUG_PRINT("Wrong component type or values.\n");

				} // for (rapidjson::SizeType j = 0; j < component.Size(); ++j) {
			} // if (object[i].HasMember("Component")) {

			else
				JE_DEBUG_PRINT("No component in object.\n");

			FACTORY::AddCreatedObject(_pOBC);
		} // if (object[i]["Name"].IsString()) {

		else // if (object[i]["Name"].IsString()) {
			JE_DEBUG_PRINT("Wrong type of object name.\n");

	} // for (rapidjson::SizeType i = 0; i < object.Size(); ++i) {
}

void JsonParser::LoadComponents(CR_RJValue _data)
{
		FACTORY::GetCreatedObject()->AddComponent(_data["Type"].GetString());
		Component* found = 
			FACTORY::GetCreatedObject()->GetComponent(_data["Type"].GetString());
		if (_data.HasMember("Values"))
			found->Load(_data["Values"]);
}

JE_END
