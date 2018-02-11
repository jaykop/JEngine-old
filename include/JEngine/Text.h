#pragma once

#include <map>
#include "ft2build.h"
#include FT_FREETYPE_H

#include "Sprite.h"
#include "Vector2.h"

JE_BEGIN

class Font {

	struct Character {
		unsigned	m_texture;	// ID handle of the glyph texture
		unsigned	m_advance;	// Horizontal offset to advance to next glyph
		vec2		m_size;		// Size of glyph
		vec2		m_bearing;		// Offset from baseline to left/top of glyph
	};

	typedef std::map<char, Character> FontData;

	friend class AssetManager;
	friend class GraphicSystem;

private:

	Font();
	~Font() {};
	Font(const Font& /*_copy*/) = delete;
	void operator = (const Font& /*_copy*/) = delete;

	FontData m_data;
	FT_Face m_face;
	FT_Library m_lib;
	unsigned m_fontSize;
	float m_newLineInterval;
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
	friend class GraphicSystem;
	friend class TextBuilder;

public:

	void Register() override;

	void				SetText(const char* _text, ...);
	const std::string&	GetText(void) const;

	Font* m_pFont;

private:
	
	char		m_textStorage[1024];
	std::string m_text;

	Text(Object* pObject);
	~Text();

	void Load(CR_RJValue _data) override;

	Text() = delete;
	Text(const Text& /*_copy*/) = delete;
	void operator=(const Text& /*_copy*/) = delete;
};

JE_END