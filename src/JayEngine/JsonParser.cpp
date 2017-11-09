#include "Object.h"
#include "Component.h"
#include "JsonParser.h"
#include "ObjectFactory.h"
#include "ObjectContainer.h"
#include "stringbuffer.h"
#include "FileReadStream.h"

JE_BEGIN

RJDoc JsonParser::m_document;

void JsonParser::ReadFile(const char * _dir)
{
	m_document.Clear();
	FILE* pFile;
	fopen_s(&pFile, _dir, "rb");
	char buffer[65536];
	rapidjson::FileReadStream read(pFile, buffer, sizeof(buffer));
	m_document.ParseStream<0, rapidjson::UTF8<>, rapidjson::FileReadStream>(read);
}

const RJDoc& JsonParser::GetDocument()
{
	return m_document;
}

void JsonParser::LoadObjects(ObjectContainer* _pOBC)
{
	CR_RJValue object = m_document["Object"];

	for (rapidjson::SizeType i = 0; i < object.Size(); ++i) {

		CR_RJValue component = object[i]["Component"];

		if (component[i]["Type"].IsString()) {
			FACTORY::CreateObject(component[i]["Type"].GetString());

			for (rapidjson::SizeType j = 0; j < component.Size(); ++j)
				LoadComponents(component[j]);
			FACTORY::AddCreatedObject(_pOBC);
		}

		else
			JE_DEBUG_PRINT("Wrong type of object name.\n");
	}
}

void JsonParser::LoadComponents(CR_RJValue _data)
{
	if (_data["Type"].IsString()) {
		FACTORY::GetCreatedObject()->AddComponent(_data["Type"].GetString());
		Component* found = 
			FACTORY::GetCreatedObject()->GetComponent(_data["Type"].GetString());
		found->Load(_data["Values"]);
	}

	else
		JE_DEBUG_PRINT("Wrong type of component name.\n");
}

JE_END
