#pragma once
#include "CustomComponent.h"
#include "ComponentBuilder.h"
#include "Location.h"
#include "Vector3.h"

jeBegin

class Text;
class Object;
class Transform;
struct Telegram;

jeDeclareCustomComponentBuilder(WifeState);
jeDeclareCustomComponentBuilder(DoHousework);
jeDeclareCustomComponentBuilder(CookStew);
jeDeclareCustomComponentBuilder(GoToBathroom);

/////////////////////////////////////////////////////////////////////////
// Global wife state
/////////////////////////////////////////////////////////////////////////
class WifeState : public CustomComponent
{
	friend class ComponentManager;
	friend class WifeStateBuilder;

public:

	Location	m_location;
	int			m_natureCalling = 0, m_chores = 10;
	int			m_minerId = 0;
	bool		m_isCooking = false;
	const char	*m_content = nullptr;

	Object*		m_wifeTalks = nullptr;
	Text*		m_talkText = nullptr;
	Transform*	m_pTransform = nullptr, *m_pTalkTransform = nullptr;
	vec3		m_talkOffset;

private:

	WifeState(Object* pObject);
	~WifeState() {};

	void Register() override;
	void Load(CR_RJValue _data) override;
	void Init() override;
	void Update(const float _dt) override;
	void Close() override;
	void Unload() override {};
	bool OnMessage(Telegram& msg) override;

	void EditorUpdate(const float /*_dt*/) override {};

};

/////////////////////////////////////////////////////////////////////////
// Housework state
/////////////////////////////////////////////////////////////////////////
class DoHousework : public CustomComponent
{
	friend class ComponentManager;
	friend class DoHouseworkBuilder;

public:

	WifeState * m_globalState = nullptr;

private:

	DoHousework(Object* pObject);
	~DoHousework() {};

	void Register() override;
	void Load(CR_RJValue _data) override;
	void Init() override;
	void Update(const float _dt) override;
	void Close() override;
	void Unload() override {};
	bool OnMessage(Telegram& msg) override;

	void EditorUpdate(const float /*_dt*/) override {};

};

/////////////////////////////////////////////////////////////////////////
// Cook Stew state
/////////////////////////////////////////////////////////////////////////
class CookStew : public CustomComponent
{
	friend class ComponentManager;
	friend class CookStewBuilder;

public:

	WifeState	*m_globalState = nullptr;

private:

	CookStew(Object* pObject);
	~CookStew() {};

	void Register() override;
	void Load(CR_RJValue _data) override;
	void Init() override;
	void Update(const float _dt) override;
	void Close() override;
	void Unload() override {};
	bool OnMessage(Telegram& msg) override;

	void EditorUpdate(const float /*_dt*/) override {};

};

/////////////////////////////////////////////////////////////////////////
// GoToBathroom state
/////////////////////////////////////////////////////////////////////////
class GoToBathroom : public CustomComponent
{
	friend class ComponentManager;
	friend class GoToBathroomBuilder;

public:

	WifeState * m_globalState = nullptr;

private:

	GoToBathroom(Object* pObject);
	~GoToBathroom() {};

	void Register() override;
	void Load(CR_RJValue _data) override;
	void Init() override;
	void Update(const float _dt) override;
	void Close() override;
	void Unload() override {};
	bool OnMessage(Telegram& msg) override;

	void EditorUpdate(const float /*_dt*/) override {};

};

jeEnd
