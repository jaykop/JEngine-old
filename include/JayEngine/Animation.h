#pragma once
#include "Timer.h"
#include "Component.h"
#include "ComponentBuilder.h"

JE_BEGIN

class AnimationBuilder : public ComponentBuilder
{

	friend class AssetManager;

public:

private:

	AnimationBuilder();
	~AnimationBuilder() {};
	AnimationBuilder(const AnimationBuilder& /*_copy*/) {};
	void operator=(const AnimationBuilder& /*_copy*/) {};

	Component* CreateComponent(Object* _pOwner) const override;

};

class Animation : public Component
{
	// Keyword Definitions
	friend class	GraphicSystem;
	friend class	ComponentManager;
	friend class	AnimationBuilder;

public:

	int		GetAnimationFrame();
	float	GetAnimationSpeed();
	bool	GetActiveAnimationToggle();
	void	ActiveAnimation(bool _toggle);
	void	FixAnimationFrame(int _thFrame);
	void	SetAnimationSpeed(float _speed);
	void	SetAnimationFrame(int _numOfFrame);

private:

	// Locked constuctors and destructor
	~Animation() {};
	Animation(Object* _owner);

	Animation() = delete;
	Animation(const Animation& /*_copy*/) = delete;
	void operator=(const Animation& /*_copy*/) = delete;

	void Load(CR_RJValue _data) override;
	void Register() override {};

	Timer	m_timer;
	int		m_animationFrames;
	int		m_animationFixFrame;
	bool	m_activeAnimation;
	float	m_realSpeed;
	float	m_realFrame;
	float	m_curretFrame;
	float	m_animationSpeed;

};

JE_END
