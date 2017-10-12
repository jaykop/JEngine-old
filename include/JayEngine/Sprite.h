#pragma once
#include "Macro.h"
#include "Timer.h"
#include "Vector4.h"
#include "Component.h"

NS_JE_BEGIN

class Object;
class Texture;
class Transform;
class Sprite : public Component
{
	// Keyword Definitions
	friend class	GraphicSystem;
	friend class	ComponentManager;
	typedef			std::unordered_map<std::string, Texture*>	TextureMap;
	enum			ProjectType { PERSPECTIVE, ORTHOGONAL };

public:
	
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
	Texture*	GetCurrentTexutre();
	Texture*	GetTexutre(const char* _key);

	vec4		m_color;
	ProjectType m_projection;

private:

	// Locked constuctors and destructor
	~Sprite();
	Sprite(Object* _owner = nullptr);
	Sprite(const Sprite& /*_copy*/) {};
	void operator=(const Sprite& /*_copy*/) {};

	Timer	m_timer;
	int		m_animationFrames;
	int		m_animationFixFrame;
	bool	m_activeAnimation;
	float	m_realSpeed;
	float	m_realFrame;
	float	m_curretFrame;
	float	m_animationSpeed;

	Texture		*m_mainTex;
	TextureMap	m_textureMap;
	Transform	*m_transform;

};

NS_JE_END