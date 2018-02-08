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

private:
	
	// TODO
	// Change to vector
	char		m_textStorage[512];
	std::string m_text;

	Text(Object* pObject);
	~Text();

	void Load(CR_RJValue _data) override;

	Text() = delete;
	Text(const Text& /*_copy*/) = delete;
	void operator=(const Text& /*_copy*/) = delete;
};

JE_END