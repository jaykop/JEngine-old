#include "Text.h"
#include "Transform.h"
#include "GLManager.h"
#include "SystemManager.h"

JE_BEGIN

Text::Text(Object* _pOwner)
	:Sprite(_pOwner), m_fontSize(32.f)
{
	m_isText = true;
	m_vao = new GLuint;// &(GLM::m_vao[GLM::SHAPE_PLANE]);
	glGenVertexArrays(1, m_vao);
	glGenBuffers(1, &m_vbo);

	glBindVertexArray(*m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glBindVertexArray(0);
}

Text::~Text()
{
	delete m_vao;
	m_vao = nullptr;

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
	va_list ap;

	if (_text == NULL)
		m_text.assign(0);

	else {
		va_start(ap, _text);
		vsprintf_s(m_textStorage, _text, ap);
		va_end(ap);

		m_text = m_textStorage;
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
}

TextBuilder::TextBuilder()
	:ComponentBuilder()
{}

Component* TextBuilder::CreateComponent(Object* _pOwner) const
{
	return new Text(_pOwner);
}

JE_END

