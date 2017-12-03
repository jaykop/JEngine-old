#pragma once
#include "Vector4.h"
#include "Component.h"
#include "ComponentBuilder.h"
#include "VisualEffect.h"

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

	using Effects		= std::unordered_map<VisualEffect::VEType, VisualEffect*>;
	using TextureMap	= std::unordered_map<std::string, unsigned>;

public:
	
	enum ProjectType { PERSPECTIVE, ORTHOGONAL };

	void Register() override;

	void		AddEffect(VisualEffect::VEType type);
	void		RemoveEffect(VisualEffect::VEType type);

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
	Sprite(Object* _owner);

	void Load(CR_RJValue _data) override;

	bool m_culled;
	bool m_hasMaterial, m_hasAnimation;

	Effects m_effects;

	unsigned	m_mainTex;
	TextureMap	m_textureMap;
	Transform	*m_transform;
	Material	*m_material;
	Animation	*m_animation;


private:

	// Locked constuctors and destructor
	Sprite() = delete;
	Sprite(const Sprite& /*_copy*/) = delete;
	void operator=(const Sprite& /*_copy*/) = delete;

};

JE_END

// TODO 
// Remove later
// #include "Sprite.inl"
