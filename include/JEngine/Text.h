#pragma once

#include <map>
#include "ft2build.h"
#include FT_FREETYPE_H

#include "Sprite.h"
#include "Vector2.h"

JE_BEGIN

struct Character {
    unsigned	m_texture;	// ID handle of the glyph texture
    unsigned	m_advance;	// Horizontal offset to advance to next glyph
    vec2		m_size;		// Size of glyph
    vec2		m_bearing;	// Offset from baseline to left/top of glyph
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

class TextBuilder : public ComponentBuilder
{

    friend class AssetManager;

public:

private:

    TextBuilder();
    ~TextBuilder() {};
    TextBuilder(const TextBuilder& /*_copy*/) = delete;
    void operator=(const TextBuilder& /*_copy*/) = delete;

    Component* CreateComponent(Object* _pOwner) const override;

};

class Text : public Sprite
{
    friend class ComponentManager;
    friend class GraphicSystem;
    friend class TextBuilder;

public:

    void Register() override;

    void				SetText(const char* _text, ...);
    const std::string&	GetText() const;

    void				SetText(const wchar_t* _wText, ...);
    const std::wstring& GetWText() const;

    Font* m_pFont;

private:

    bool m_printWide = false;

    wchar_t			*m_wTextStorage = nullptr;
    std::wstring	m_wText;

    char			*m_textStorage = nullptr;
    std::string		m_text;

	size_t			m_size = 0;

    Text(Object* pObject);
    ~Text();
    void operator=(const Text& _copy);
    void Load(CR_RJValue _data) override;
    void EditorUpdate(const float _dt) override;

    Text() = delete;
    Text(const Text& /*_copy*/) = delete;
    Text(Text&& /*_copy*/) = delete;
};

JE_END