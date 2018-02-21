#include "Text.h"
#include "Transform.h"
#include "GLManager.h"
#include "AssetManager.h"
#include "SystemManager.h"

JE_BEGIN

Font::Font()
	:m_fontSize(0), m_newLineInterval(0.f)
{}

Text::Text(Object* _pOwner)
	:Sprite(_pOwner)
{
	m_isText = true;
}

Text::~Text()
{
	SYSTEM::GetGraphicSystem()->RemoveSprite(this);
}

void Text::Register()
{
	SYSTEM::GetGraphicSystem()->AddSprite(this);
	if (m_pOwner->HasComponent<Transform>())
		m_transform = m_pOwner->GetComponent<Transform>();
}

void Text::SetText(const char * _text, ...)
{
	if (_text == NULL)
		m_text.assign(0);

	else {
		va_list ap;

		va_start(ap, _text);
		vsprintf_s(m_textStorage, _text, ap);
		va_end(ap);

		m_text = m_textStorage;
		if (m_text.length() > 1024)
			JE_DEBUG_PRINT("!Text - Too long text content. Must be shorter than 1024.\n");
	}
}

const std::string& Text::GetText(void) const
{
	return m_text;
}

void Text::Load(CR_RJValue _data)
{
	if (_data.HasMember("Text")) {
		CR_RJValue text = _data["Text"];
		m_text.assign(text.GetString());
	}

	if (_data.HasMember("Font")) {
		CR_RJValue font = _data["Font"];
		m_pFont = ASSET::GetFont(font.GetString());
	}

	if (_data.HasMember("Color")) {
		CR_RJValue color = _data["Color"];
		m_color.Set(color[0].GetFloat(), color[1].GetFloat(),
			color[2].GetFloat(), color[3].GetFloat());
	}

	if (_data.HasMember("Projection")) {
		CR_RJValue projection = _data["Projection"];

		if (!strcmp("Perspective", projection.GetString())) {
			m_projection = PROJECTION_PERSPECTIVE;
		}

		else if (!strcmp("Orthogonal", projection.GetString())) {
			m_projection = PROJECTION_ORTHOGONAL;
		}
		else
			JE_DEBUG_PRINT("!Sprite - Wrong projection type: %s\n", projection.GetString());
	}
}

TextBuilder::TextBuilder()
	:ComponentBuilder()
{}

Component* TextBuilder::CreateComponent(Object* _pOwner) const
{
	return new Text(_pOwner);
}

JE_END
