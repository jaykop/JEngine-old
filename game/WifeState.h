#pragma once
#include "CustomComponent.h"
#include "ComponentManager.h"
#include "ComponentBuilder.h"
#include "Location.h"
#include "Vector3.h"

jeBegin

class Text;
class Object;
class Transform;
struct Telegram;

/////////////////////////////////////////////////////////////////////////
// Global wife state
/////////////////////////////////////////////////////////////////////////
class WifeState : public CustomComponent
{
    jeBaseFriends(WifeState);

public:

    Location	m_location;
    int		m_natureCalling = 0, m_chores = 10;
    int		m_minerId = 0;
    bool	m_isCooking = false;
    const char	*m_content = nullptr;

    Object*	m_wifeTalks = nullptr;
    Text*	m_talkText = nullptr;
    Transform*	m_pTransform = nullptr, *m_pTalkTransform = nullptr;
    vec3	m_talkOffset;

private:

    WifeState(Object* pObject);
    ~WifeState() {};

    void Register() override;
    void Load(CR_RJValue _data) override;
    void Init() override;
    void Update(float dt) override;
    void Close() override;
    void Unload() override {};
    bool OnMessage(Telegram& msg) override;

    void EditorUpdate(const float /*dt*/) override {};

};

/////////////////////////////////////////////////////////////////////////
// Housework state
/////////////////////////////////////////////////////////////////////////
class DoHousework : public CustomComponent
{
    jeBaseFriends(DoHousework);

public:

    WifeState * m_globalState = nullptr;

private:

    DoHousework(Object* pObject);
    ~DoHousework() {};

    void Register() override;
    void Load(CR_RJValue _data) override;
    void Init() override;
    void Update(float dt) override;
    void Close() override;
    void Unload() override {};
    bool OnMessage(Telegram& msg) override;

    void EditorUpdate(const float /*dt*/) override {};

};

/////////////////////////////////////////////////////////////////////////
// Cook Stew state
/////////////////////////////////////////////////////////////////////////
class CookStew : public CustomComponent
{
    jeBaseFriends(CookStew);

public:

    WifeState * m_globalState = nullptr;

private:

    CookStew(Object* pObject);
    ~CookStew() {};

    void Register() override;
    void Load(CR_RJValue _data) override;
    void Init() override;
    void Update(float dt) override;
    void Close() override;
    void Unload() override {};
    bool OnMessage(Telegram& msg) override;

    void EditorUpdate(const float /*dt*/) override {};

};

/////////////////////////////////////////////////////////////////////////
// GoToBathroom state
/////////////////////////////////////////////////////////////////////////
class GoToBathroom : public CustomComponent
{
    jeBaseFriends(GoToBathroom);

public:

    WifeState * m_globalState = nullptr;

private:

    GoToBathroom(Object* pObject);
    ~GoToBathroom() {};

    void Register() override;
    void Load(CR_RJValue _data) override;
    void Init() override;
    void Update(float dt) override;
    void Close() override;
    void Unload() override {};
    bool OnMessage(Telegram& msg) override;

    void EditorUpdate(const float /*dt*/) override {};

};

jeDeclareCustomComponentBuilder(WifeState);
jeDeclareCustomComponentBuilder(DoHousework);
jeDeclareCustomComponentBuilder(CookStew);
jeDeclareCustomComponentBuilder(GoToBathroom);

jeEnd
