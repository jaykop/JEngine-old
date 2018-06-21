#pragma once

#include <map>
#include "ft2build.h"
#include FT_FREETYPE_H

#include "Model.h"
#include "Vector2.h"

jeBegin
jeDeclareComponentBuilder(Text);

struct Character {
    unsigned	texture;	// ID handle of the glyph texture
    unsigned	advance;	// Horizontal offset to advance to next glyph
    vec2		size;		// Size of glyph
    vec2		bearing;	// Offset from baseline to left/top of glyph
};

class Font {

    friend class AssetManager;
    friend class GraphicSystem;

    using FontData = std::map<unsigned long, Character>;

private:

    Font();
    ~Font() {};
    Font(const Font& /*_copy*/) = delete;
    void operator= (const Font& /*_copy*/) = delete;
    void operator= (Font&& /*_copy*/) = delete;

    FontData	m_data;
    FT_Face		m_face;
    FT_Library	m_lib;
    unsigned	m_fontSize;
    float		m_newLineInterval;
};

class Text : public Model
{
    jeBaseFriends(Text);
    friend class GraphicSystem;

public:

    void Register() override;

    void SetText(const char* _text, ...);
    const std::string&	GetText() const;

    void SetText(const wchar_t* _wText, ...);
    const std::wstring& GetWText() const;

    Font* pFont;

private:

    bool			m_printWide = false;
	char			*m_pTextStorage = nullptr;
    wchar_t	*m_pwTextStorage = nullptr;
	std::string		m_text;
    std::wstring	m_wText;
    size_t			m_size = 0;

	static std::vector<unsigned> m_idices;

    Text(Object* pObject);
    ~Text();
    void operator=(const Text& _copy);
    void Load(CR_RJValue _data) override;
    void EditorUpdate(const float _dt) override;

    Text() = delete;
    Text(const Text& /*_copy*/) = delete;
    Text(Text&& /*_copy*/) = delete;
};

jeEnd
