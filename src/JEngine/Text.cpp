#include "Text.h"
#include "Transform.h"
#include "GLManager.h"
#include "AssetManager.h"
#include "SystemManager.h"
#include "Object.h"

#ifdef  jeUseBuiltInAllocator
#include "MemoryAllocator.h"
#endif

jeBegin
jeDefineComponentBuilder(Text);

std::vector<unsigned> Text::m_idices = { 0, 2, 3, 1, 0, 2 };

Font::Font()
	:m_fontSize(0), m_newLineInterval(0.f)
{}

Text::Text(Object* _pOwner)
	:Model(_pOwner)
{
	m_hiddenStatus |= IS_TEXT;
}

Text::~Text()
{
	if (m_pTextStorage) {
		delete[] m_pTextStorage;
		m_pTextStorage = nullptr;
	}

	if (m_pwTextStorage) {
		delete[] m_pwTextStorage;
		m_pwTextStorage = nullptr;
	}

	SYSTEM::GetGraphicSystem()->RemoveModel(this);
}

void Text::operator=(const Text & _copy)
{
	pFont = _copy.pFont;
	m_text = _copy.m_text;
	m_wText = _copy.m_wText;
	m_printWide = _copy.m_printWide;
}

void Text::Register()
{
	SYSTEM::GetGraphicSystem()->AddModel(this);
	if (GetOwner()->HasComponent<Transform>())
		m_pTransform = GetOwner()->GetComponent<Transform>();
}

void Text::SetText(const char * _text, ...)
{
	// Set render to print ascii characters
	m_printWide = false;

	// Clear wide character conatiner
	if (!m_wText.empty()) {
		m_wText.clear();
		delete[] m_pwTextStorage;
		m_pwTextStorage = nullptr;
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
			delete[] m_pTextStorage;			// Delete heap
			m_pTextStorage = nullptr;
			m_pTextStorage = new char[m_size];	// Reallocate memory
		}

		vsprintf_s(m_pTextStorage, m_size, _text, argumens);
		va_end(argumens);

		// Refresh the text with additional arguments
		m_text = m_pTextStorage;
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
		delete[] m_pTextStorage;
		m_pTextStorage = nullptr;
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
			delete[] m_pwTextStorage;				// Deallocate heap memories
			m_pwTextStorage = nullptr;
			m_pwTextStorage = new wchar_t[m_size];	// Reallocate new memories
		}

		vswprintf_s(m_pwTextStorage, m_size, _wText, argumens);
		va_end(argumens);

		// Refresh the text with additional arguments
		m_wText = m_pwTextStorage;
	}
}

const std::wstring& Text::GetWText() const
{
	return m_wText;
}

void Text::Load(CR_RJValue _data)
{
	if (_data.HasMember("Text")) {
		CR_RJValue loadedText = _data["Text"];
		m_text.assign(loadedText.GetString());
	}

	if (_data.HasMember("Font")) {
		CR_RJValue loadedFont = _data["Font"];
		pFont = ASSET::GetFont(loadedFont.GetString());
	}

	if (_data.HasMember("Color")) {
		CR_RJValue loadedColor = _data["Color"];
		color.Set(loadedColor[0].GetFloat(), loadedColor[1].GetFloat(),
			loadedColor[2].GetFloat(), loadedColor[3].GetFloat());
	}

	if (_data.HasMember("Projection")) {
		CR_RJValue loadedProjection = _data["Projection"];

		if (!strcmp("Perspective", loadedProjection.GetString())) {
			projection = PROJECTION_PERSPECTIVE;
		}

		else if (!strcmp("Orthogonal", loadedProjection.GetString())) {
			projection = PROJECTION_ORTHOGONAL;
		}
		else
			jeDebugPrint("!Model - Wrong projection type: %s\n", loadedProjection.GetString());
	}
}

void Text::EditorUpdate(const float /*_dt*/)
{
	// TODO
}

jeEnd

