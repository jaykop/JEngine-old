#pragma once
#include "Macro.h"
#include "Timer.h"
#include "Vector4.h"
#include "Component.h"

NS_JE_BEGIN

class Object;
class Texture;
class Sprite : public Component
{
	// Keyword Definitions
	friend class GraphicSystem;
	friend class ComponentManager;
	typedef std::unordered_map<std::string, Texture*>	TextureMap;
	enum ProjectType { PERSPECTIVE, ORTHOGONAL };

public:

	bool	m_activeAnimation;
	
	vec4		m_color;
	Timer		m_timer;
	ProjectType m_projection;

	void FixAnimationFrame(int _thFrame);
	void SetAnimationFrame(int _numOfFrame);
	void SetAnimationSpeed(float _speed);

	void		AddTexture(const char* _key);
	void		RemoveTexture(const char* _key);
	void		SetCurrentTexutre(const char* _key);
	Texture*	GetCurrentTexutre();
	Texture*	GetTexutre(const char* _key);

private:

	// Locked constuctors and destructor
	~Sprite();
	Sprite(Object* _owner);
	Sprite(const Sprite& /*_copy*/) {};
	void operator=(const Sprite& /*_copy*/) {};

	float m_curretFrame;
	float m_animationSpeed;
	float m_animationFrames;
	float m_animationFixFrame;

	float m_realSpeed;
	float m_realFrame;

	Texture		*m_mainTex;
	TextureMap	m_textureMap;

};

NS_JE_END