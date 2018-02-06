#include "Animation.h"
#include "SystemManager.h"
#include "GraphicSystem.h"
#include "Sprite.h"

JE_BEGIN

Animation::Animation(Object* _pOwner)
	: Component(_pOwner), m_currentFrame(0.f), m_animationSpeed(0.f),
	m_animationFrames(1), m_animationFixFrame(1), m_realSpeed(0.f),
	m_realFrame(1.f), m_activeAnimation(false)
{	
	// Connect to sprite's pointer
	if (_pOwner->HasComponent<Sprite>()
		&& !_pOwner->GetComponent<Sprite>()->m_hasAnimation) {
		_pOwner->GetComponent<Sprite>()->m_animation = this;
		_pOwner->GetComponent<Sprite>()->m_hasAnimation = true;
	}

	else
		JE_DEBUG_PRINT("!Animation - This object has no sprite componnet: %s\n", _pOwner->GetName().c_str());
}

Animation::~Animation() {

	// Turn off the toggle
	if (m_pOwner->HasComponent<Sprite>()) {
		m_pOwner->GetComponent<Sprite>()->m_animation = nullptr;
		m_pOwner->GetComponent<Sprite>()->m_hasAnimation = false;
	}
}

void Animation::Load(CR_RJValue _data)
{
	if (_data.HasMember("FixAt")) {
		CR_RJValue fix = _data["FixAt"];
		FixAnimationFrame(fix.GetInt());
	}

	if (_data.HasMember("Active")) {
		CR_RJValue active = _data["Active"];
		m_activeAnimation = active.GetBool();
	}

	if (_data.HasMember("Frame")) {
		CR_RJValue frame = _data["Frame"];
		SetAnimationFrame(frame.GetInt());
	}

	if (_data.HasMember("Speed")) {
		CR_RJValue speed = _data["Speed"];
		SetAnimationSpeed(speed.GetFloat());
	}
}

bool Animation::GetActiveAnimationToggle()
{
	return m_activeAnimation;
}

void Animation::ActiveAnimation(bool _toggle)
{
	m_activeAnimation = _toggle;
	if (m_activeAnimation)
		m_timer.Start();
}

void Animation::FixAnimationFrame(int _thFrame)
{
	m_animationSpeed = 0.f;
	m_activeAnimation = false;
	m_currentFrame = float(_thFrame) * m_realFrame;
}

void Animation::SetAnimationFrame(int _numOfFrame)
{
	m_animationFrames = _numOfFrame;
	m_realFrame = 1.f / float(m_animationFrames);
}

void Animation::SetAnimationSpeed(float _speed)
{
	m_animationSpeed = _speed;
	m_realSpeed = 1.f / _speed;
}

int Animation::GetAnimationFrame()
{
	return m_animationFrames;
}

float Animation::GetAnimationSpeed()
{
	return m_animationSpeed;
}

AnimationBuilder::AnimationBuilder()
	:ComponentBuilder()
{}

Component* AnimationBuilder::CreateComponent(Object* _pOwner) const
{
	return new Animation(_pOwner);
}

JE_END