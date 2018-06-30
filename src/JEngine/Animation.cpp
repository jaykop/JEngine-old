#include "Animation.h"
#include "GraphicSystem.h"
#include "Model.h"
#include "Object.h"

#ifdef  jeUseBuiltInAllocator
#include "MemoryAllocator.h"
#endif

jeBegin
jeDefineComponentBuilder(Animation);

Animation::Animation(Object* pOwner)
	: Component(pOwner), m_currentFrame(0.f), m_animationSpeed(0.f),
	m_animationFrames(1), m_animationFixFrame(1), m_realSpeed(0.f),
	m_realFrame(1.f), active_Animation(false)
{	
	// Connect to model's pointer
	if (pOwner->HasComponent<Model>()) 
		pOwner->GetComponent<Model>()->m_pAnimation = this;

	else
		jeDebugPrint("!Animation - This object has no model componnet: %s\n", pOwner->GetName().c_str());
}

Animation::~Animation() {

	// Turn off the toggle
	if (GetOwner()->HasComponent<Model>()) 
		GetOwner()->GetComponent<Model>()->m_pAnimation = nullptr;
}

void Animation::operator=(const Animation & copy)
{
	m_currentFrame = copy.m_currentFrame; 
	m_animationSpeed = copy.m_animationSpeed;
	m_animationFrames = copy.m_animationFrames;
	m_animationFixFrame = copy.m_animationFixFrame;
	m_realSpeed = copy.m_realSpeed;
	m_realFrame = copy.m_realFrame;
	active_Animation = copy.active_Animation;

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
		active_Animation = active.GetBool();
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

void Animation::EditorUpdate(const float /*dt*/)
{
	// TODO
}

bool Animation::GetActiveAnimationToggle() const
{ 
	return active_Animation;
}

void Animation::ActiveAnimation(bool _toggle)
{
	active_Animation = _toggle;
	if (active_Animation)
		timer_.Start();
}

void Animation::FixAnimationFrame(int _thFrame)
{
	m_animationSpeed = 0.f;
	active_Animation = false;
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
