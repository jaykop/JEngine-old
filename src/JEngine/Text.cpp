#include "Text.h"
#include "Transform.h"
#include "GLManager.h"
#include "AssetManager.h"
#include "SystemManager.h"
#include "MemoryAllocator.h"

jeBegin

jeDefineStaticAllocator(Text);

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
	if (m_textStorage) {
		delete[] m_textStorage;
		m_textStorage = nullptr;
	}

	if (m_wTextStorage) {
		delete[] m_wTextStorage;
		m_wTextStorage = nullptr;
	}

	SYSTEM::GetGraphicSystem()->RemoveSprite(this);
}

void Text::operator=(const Text & _copy)
{
	m_pFont = _copy.m_pFont;
	m_text = _copy.m_text;
	m_wText = _copy.m_wText;
	m_printWide = _copy.m_printWide;
}

void Text::Register()
{
	SYSTEM::GetGraphicSystem()->AddSprite(this);
	if (m_pOwner->HasComponent<Transform>())
		m_transform = m_pOwner->GetComponent<Transform>();
}

void Text::SetText(const char * _text, ...)
{
	// Set render to print ascii characters
	m_printWide = false;

	// Clear wide character conatiner
	if (!m_wText.empty()) {
		m_wText.clear();
		delete[] m_wTextStorage;
		m_wTextStorage = nullptr;
	}

	if (_text)
	{
		static unsigned newSize = 0;
		m_text.assign(_text); 
		va_list argumens;

		va_start(argumens, _text);
		
		// Get size of new text
		newSize = _vscprintf(_text, argumens) + 1;

		// If the new size is greater than old one,
		// delete existing one and reallocate heap memories
		if (m_size < newSize) {
			m_size = newSize;					// Refresh the size info
			delete[] m_textStorage;				// Delete heap
			m_textStorage = nullptr;
			m_textStorage = new char[m_size];	// Reallocate memory
		}

		vsprintf_s(m_textStorage, m_size, _text, argumens);
		va_end(argumens);

		// Refresh the text with additional arguments
		m_text = m_textStorage;
	}
}

const std::string& Text::GetText(void) const
{
	return m_text;
}

void Text::SetText(const wchar_t* _wText, ...)
{
	// Set render to print wide characters
	m_printWide = true;

	// Clear ascii texts container
	if (!m_text.empty()) {
		m_text.clear();
		delete[] m_textStorage;
		m_textStorage = nullptr;
	}

	if (_wText)
	{
		static size_t newSize = 0;
		m_wText.assign(_wText);
		va_list argumens;

		va_start(argumens, _wText);
		// Get new size of the new text
		newSize = _vscwprintf(_wText, argumens) + 1;

		// If new size is greater than one,
		// reallocate the new heap memories
		if (m_size < newSize) {
			m_size = newSize;						// Initialize new size
			delete[] m_wTextStorage;				// Deallocate heap memories
			m_wTextStorage = nullptr;
			m_wTextStorage = new wchar_t[m_size];	// Reallocate new memories
		}

		vswprintf_s(m_wTextStorage, m_size, _wText, argumens);
		va_end(argumens);

		// Refresh the text with additional arguments
		m_wText = m_wTextStorage;
	}
}

const std::wstring& Text::GetWText() const
{
	return m_wText;
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
			jeDebugPrint("!Sprite - Wrong projection type: %s\n", projection.GetString());
	}
}

void Text::EditorUpdate(const float /*_dt*/)
{
	// TODO
}

TextBuilder::TextBuilder()
	:ComponentBuilder()
{}

Component* TextBuilder::CreateComponent(Object* _pOwner) const
{
	return new Text(_pOwner);
}

jeEnd

