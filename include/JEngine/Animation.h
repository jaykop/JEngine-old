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
    void    ActiveAnimation(bool _toggle);
    void    FixAnimationFrame(int _thFrame);
    void    SetAnimationSpeed(float _speed);
    void    SetAnimationFrame(int _numOfFrame);

private:

    // Locked constuctors and destructor
	virtual ~Animation();
    Animation(Object* pOwner);
    void operator=(const Animation& copy);

    Animation() = delete;
    Animation(const Animation& /*copy*/) = delete;

    void Load(CR_RJValue _data) override;
    void Register() override {};

    Timer   timer_;
    int	    m_animationFrames;
    int	    m_animationFixFrame;
    bool    active_Animation;
    float   m_realSpeed;
    float   m_realFrame;
    float   m_currentFrame;
    float   m_animationSpeed;

    void    EditorUpdate(float dt) override;

};

jeDeclareComponentBuilder(Animation);

jeEnd
