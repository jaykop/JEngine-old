#pragma once
#include "Macro.h"
#include "Timer.h"
#include "Vector4.h"
#include "Component.h"
#include "ComponentBuilder.h"

JE_BEGIN

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

class Object;
class Material; 
class Transform;
class Sprite : public Component
{
	// Keyword Definitions
	friend class	Material;
	friend class	AssetManager;
	friend class	GraphicSystem;
	friend class	ComponentManager;
	friend class	SpriteBuilder;
	typedef			std::unordered_map<std::string, unsigned>	TextureMap;

public:
	
	enum	ProjectType { PERSPECTIVE, ORTHOGONAL };

	int		GetAnimationFrame();
	float	GetAnimationSpeed();
	bool	GetActiveAnimationToggle();
	void	ActiveAnimation(bool _toggle);
	void	FixAnimationFrame(int _thFrame);
	void	SetAnimationSpeed(float _speed);
	void	SetAnimationFrame(int _numOfFrame);

	void		AddTexture(const char* _key);
	void		RemoveTexture(const char* _key);
	void		SetCurrentTexutre(const char* _key);
	unsigned	GetCurrentTexutre();
	unsigned	GetTexutre(const char* _key);
	//Transform*	GetTransform();

	bool		m_flip;
	vec4		m_color;
	ProjectType m_projection;

private:

	// Locked constuctors and destructor
	~Sprite();
	Sprite(Object* _owner = nullptr);
	Sprite(const Sprite& /*_copy*/) {};
	void operator=(const Sprite& /*_copy*/) {};

	void Load(const RJValue& _data) override;
	void Init() override {};
	void Update(float /*_dt*/) override {};
	void Close() override {};
	void Unload() override {};

	bool m_culled;

	Timer	m_timer;
	int		m_animationFrames;
	int		m_animationFixFrame;
	bool	m_activeAnimation, m_hasMaterial;
	float	m_realSpeed;
	float	m_realFrame;
	float	m_curretFrame;
	float	m_animationSpeed;

	unsigned	m_mainTex;
	TextureMap	m_textureMap;
	Transform	*m_transform;
	Material	*m_material;

};

JE_END
