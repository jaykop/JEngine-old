#pragma once

#include "Sprite.h"

JE_BEGIN

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

	float m_fontSize;

private:
	
	// TODO
	// Change to vector
	char		m_textStorage[512];
	std::string m_text;
	unsigned	m_vbo;

	Text(Object* pObject);
	~Text();

	void Load(CR_RJValue _data) override;

	Text() = delete;
	Text(const Text& /*_copy*/) = delete;
	void operator=(const Text& /*_copy*/) = delete;
};

JE_END