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
	: Component(pOwner), currentFrame_(0.f), animationSpeed_(0.f),
	animationFrames_(1), animationFixFrame_(1), realSpeed_(0.f),
	realFrame_(1.f), active_(false)
{	
	// Connect to model's pointer
	if (pOwner->HasComponent<Model>()) 
		pOwner->GetComponent<Model>()->pAnimation_ = this;

	else
		jeDebugPrint("!Animation - This object has no model componnet: %s\n", pOwner->GetName().c_str());
}

Animation::~Animation() {

	// Turn off the toggle
	if (GetOwner()->HasComponent<Model>()) 
		GetOwner()->GetComponent<Model>()->pAnimation_ = nullptr;
}

void Animation::operator=(const Animation & copy)
{
	currentFrame_ = copy.currentFrame_; 
	animationSpeed_ = copy.animationSpeed_;
	animationFrames_ = copy.animationFrames_;
	animationFixFrame_ = copy.animationFixFrame_;
	realSpeed_ = copy.realSpeed_;
	realFrame_ = copy.realFrame_;
	active_ = copy.active_;

	// Connect to model's pointer
	if (GetOwner()->HasComponent<Model>()) 
		GetOwner()->GetComponent<Model>()->pAnimation_ = this;
}

void Animation::Load(CR_RJValue data)
{
	if (data.HasMember("FixAt")) {
		CR_RJValue fix = data["FixAt"];
		FixAnimationFrame(fix.GetInt());
	}

	if (data.HasMember("Active")) {
		CR_RJValue active = data["Active"];
		active_ = active.GetBool();
	}

	if (data.HasMember("Frame")) {
		CR_RJValue frame = data["Frame"];
		SetAnimationFrame(frame.GetInt());
	}

	if (data.HasMember("Speed")) {
		CR_RJValue speed = data["Speed"];
		SetAnimationSpeed(speed.GetFloat());
	}
}

void Animation::EditorUpdate(const float /*dt*/)
{
	// TODO
}

bool Animation::GetActiveAnimationToggle() const
{ 
	return active_;
}

void Animation::ActiveAnimation(bool toggle)
{
	active_ = toggle;
	if (active_)
		timer_.Start();
}

void Animation::FixAnimationFrame(int thFrame)
{
	animationSpeed_ = 0.f;
	active_ = false;
	currentFrame_ = float(thFrame) * realFrame_;
}

void Animation::SetAnimationFrame(int numOfFrame)
{
	animationFrames_ = numOfFrame;
	realFrame_ = 1.f / float(animationFrames_);
}

void Animation::SetAnimationSpeed(float speed)
{
	animationSpeed_ = speed;
	realSpeed_ = 1.f / speed;
}

int Animation::GetAnimationFrame() const
{
	return animationFrames_;
}

float Animation::GetAnimationSpeed() const
{
	return animationSpeed_;
}

jeEnd
