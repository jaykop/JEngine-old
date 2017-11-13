#pragma once
#include "Vector4.h"
#include "Component.h"
#include "ComponentBuilder.h"

JE_BEGIN

class Material;
class Transform;
class Animation; 
class SpriteBuilder : public ComponentBuilder
{
	friend class AssetManager;

public:

private:

	SpriteBuilder();
	~SpriteBuilder() {};
	SpriteBuilder(const SpriteBuilder& /*_copy*/) {};
	void operator=(const SpriteBuilder& /*_copy*/) {};

	Component* CreateComponent(Object* _pOwner) const override;

};

class Sprite : public Component
{
	// Keyword Definitions
	friend class	Material;
	friend class	Animation;

	friend class	ModelBuilder;
	friend class	SpriteBuilder;
	friend class	GraphicSystem;
	friend class	ComponentManager;
	typedef			std::unordered_map<std::string, unsigned>	TextureMap;

public:
	
	enum	ProjectType { PERSPECTIVE, ORTHOGONAL };

	void Register() override;

	void		AddTexture(const char* _key);
	void		RemoveTexture(const char* _key);
	void		SetCurrentTexutre(const char* _key);
	unsigned	GetCurrentTexutre();
	unsigned	GetTexutre(const char* _key);

	bool		m_flip, m_isModel;
	vec4		m_color;
	ProjectType m_projection;

protected:

	~Sprite();
	Sprite(Object* _owner = nullptr);

	void Load(CR_RJValue _data) override;

	bool m_culled;
	bool m_hasMaterial, m_hasAnimation;

	unsigned	m_mainTex;
	TextureMap	m_textureMap;
	Transform	*m_transform;
	Material	*m_material;
	Animation	*m_animation;

private:

	// Locked constuctors and destructor
	Sprite(const Sprite& /*_copy*/) {};
	void operator=(const Sprite& /*_copy*/) {};

};

JE_END
