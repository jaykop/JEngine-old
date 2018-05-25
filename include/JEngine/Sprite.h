#pragma once
#include "Vector4.h"
#include "Component.h"
#include "ComponentManager.h"
#include "ComponentBuilder.h"

// For enum ProjectType
#include "GraphicSystem.h"

jeBegin
jeDeclareComponentBuilder(Sprite);

class Transform;

class Sprite : public Component
{
	template <class T>
	friend class MemoryAllocator;

	// Keyword Definitions
	friend class	Material;
	friend class	Animation;

	friend class	ModelBuilder;
	friend class	SpriteBuilder;
	friend class	GraphicSystem;
	friend class	AssetManager;
	friend class	ComponentManager;

	using Image			= std::vector<unsigned char>;
	using TextureMap	= std::unordered_map<std::string, unsigned>;

public:

	void Register() override;

	void		AddTexture(const char* _key);
	void		RemoveTexture(const char* _key);
	void		SetCurrentTexutre(const char* _key);
	unsigned	GetCurrentTexutre();
	unsigned	GetTexutre(const char* _key);

	bool		m_flip, m_bilboard;
	vec4		m_color;
	ProjectType m_projection;

	unsigned	m_sfactor, m_dfactor;
	unsigned	*m_vao, m_elementSize;

protected:

	~Sprite();
	Sprite(Object* _pOwner);
	void operator=(const Sprite& _copy);

	void Load(CR_RJValue _data) override;

	bool m_culled;
	bool m_hasMaterial, m_hasAnimation, m_isEmitter, m_isText;

	unsigned	m_mainTex;
	TextureMap	m_textureMap;
	Transform	*m_transform;
	Material	*m_material;
	Animation	*m_animation;

private:

	// Locked constuctors and destructor
	Sprite() = delete;
	Sprite(const Sprite& /*_copy*/) = delete;

	void EditorUpdate(const float _dt) override;
};

jeEnd
