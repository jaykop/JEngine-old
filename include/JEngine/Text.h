#pragma once

#include <map>
#include "ft2build.h"
#include FT_FREETYPE_H

#include "Model.h"
#include "Mesh.h"

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
    Font(const Font& /*copy*/) = delete;
    void operator= (const Font& /*copy*/) = delete;
    void operator= (Font&& /*copy*/) = delete;

    FontData	data_;
    FT_Face		face_;
    FT_Library	lib_;
    unsigned	fontSize_;
    float		newline_;
};

class Text : public Model
{
    jeBaseFriends(Text);
    friend class GraphicSystem;

public:

    void Register() override;

    void SetText(const char* pText, ...);
    const std::string&	GetText() const;

    void SetText(const wchar_t* pText, ...);
    const std::wstring& GetWText() const;

    Font* pFont;

private:

    bool			printWide_ = false;
	char			*pTextBuffer_ = nullptr;
    wchar_t			*pwTextBuffer = nullptr;
	std::string		text_;
    std::wstring	wText_;
    size_t			size_ = 0;

	static std::vector<unsigned> m_pointIndices;

    Text(Object* pObject);
	virtual ~Text();
    void operator=(const Text& copy);
    void Load(CR_RJValue data) override;
    void EditorUpdate(float dt) override;

    Text() = delete;
    Text(const Text& /*copy*/) = delete;
    Text(Text&& /*copy*/) = delete;
};

jeEnd
