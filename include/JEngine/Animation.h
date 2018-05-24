#pragma once
#include "Timer.h"
#include "Component.h"
#include "ComponentBuilder.h"

jeBegin

template <class T>
class MemoryAllocator;

jeDeclareComponentBuilder(Animation);

class Animation : public Component
{
	// Keyword Definitions
	jeDeclareStaticAllocator(Animation);
	friend class ComponentManager;
	friend class GraphicSystem;
	friend class AnimationBuilder;

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
	~Animation();
	Animation(Object* _pOwner);
	void operator=(const Animation& _copy);

	Animation() = delete;
	Animation(const Animation& /*_copy*/) = delete;

	void Load(CR_RJValue _data) override;
	void Register() override {};

	Timer	m_timer;
	int		m_animationFrames;
	int		m_animationFixFrame;
	bool	m_activeAnimation;
	float	m_realSpeed;
	float	m_realFrame;
	float	m_currentFrame;
	float	m_animationSpeed;

	void	EditorUpdate(const float _dt) override;

};

jeEnd
