#include "Animation.h"
#include "GraphicSystem.h"
#include "Model.h"
#include "Object.h"

#ifdef  jeUseBuiltInAllocator
#include "MemoryAllocator.h"
#endif

jeBegin
jeDefineComponentBuilder(Animation);

Animation::Animation(Object* _pOwner)
	: Component(_pOwner), m_currentFrame(0.f), m_animationSpeed(0.f),
	m_animationFrames(1), m_animationFixFrame(1), m_realSpeed(0.f),
	m_realFrame(1.f), m_activeAnimation(false)
{	
	// Connect to model's pointer
	if (_pOwner->HasComponent<Model>()) 
		_pOwner->GetComponent<Model>()->m_pAnimation = this;

	else
		jeDebugPrint("!Animation - This object has no model componnet: %s\n", _pOwner->GetName().c_str());
}

Animation::~Animation() {

	// Turn off the toggle
	if (GetOwner()->HasComponent<Model>()) 
		GetOwner()->GetComponent<Model>()->m_pAnimation = nullptr;
}

void Animation::operator=(const Animation & _copy)
{
	m_currentFrame = _copy.m_currentFrame; 
	m_animationSpeed = _copy.m_animationSpeed;
	m_animationFrames = _copy.m_animationFrames;
	m_animationFixFrame = _copy.m_animationFixFrame;
	m_realSpeed = _copy.m_realSpeed;
	m_realFrame = _copy.m_realFrame;
	m_activeAnimation = _copy.m_activeAnimation;

	// Connect to model's pointer
	if (GetOwner()->HasComponent<Model>()) 
		GetOwner()->GetComponent<Model>()->m_pAnimation = this;
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

void Animation::EditorUpdate(const float /*_dt*/)
{
	// TODO
}

bool Animation::GetActiveAnimationToggle() const
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

int Animation::GetAnimationFrame() const
{
	return m_animationFrames;
}

float Animation::GetAnimationSpeed() const
{
	return m_animationSpeed;
}

jeEnd
