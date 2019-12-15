#pragma once
#include "Timer.h"
#include "Component.h"
#include "ComponentManager.h"
#include "ComponentBuilder.h"

jeBegin

class Animation : public Component
{
    // Keyword Definitions
    jeBaseFriends(Animation);
    friend class GraphicSystem;

public:

    int	    GetAnimationFrame() const;
    float   GetAnimationSpeed() const;
    bool    GetActiveAnimationToggle() const;
    void    ActiveAnimation(bool toggle);
    void    FixAnimationFrame(int thFrame);
    void    SetAnimationSpeed(float speed);
    void    SetAnimationFrame(int numOfFrame);

private:

    // Locked constuctors and destructor
	virtual ~Animation();
    Animation(Object* pOwner);
    void operator=(const Animation& copy);

    Animation() = delete;
    Animation(const Animation& /*copy*/) = delete;

    void Load(CR_RJValue data) override;
    void Register() override {};

    Timer   timer_;
    int	    animationFrames_;
    int	    animationFixFrame_;
    bool    active_;
    float   realSpeed_;
    float   realFrame_;
    float   currentFrame_;
    float   animationSpeed_;

    void    EditorUpdate(float dt) override;

};

jeDeclareComponentBuilder(Animation);

jeEnd
