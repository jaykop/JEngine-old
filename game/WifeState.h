#pragma once
#include "CustomComponent.h"
#include "ComponentBuilder.h"
#include "Location.h"

JE_BEGIN

class Object;
struct Telegram;

JE_DECLARE_COMPONENT_BUILDER(WifeState)
JE_DECLARE_COMPONENT_BUILDER(DoHousework)
JE_DECLARE_COMPONENT_BUILDER(CookStew)
JE_DECLARE_COMPONENT_BUILDER(GoToBathroom)

/////////////////////////////////////////////////////////////////////////
// Global wife state
/////////////////////////////////////////////////////////////////////////
class WifeState : public CustomComponent
{
	friend class ComponentManager;
	friend class WifeStateBuilder;

public:

	Location	m_location;
	int			m_natureCalling, m_chores;
	int			m_minerId = 0;
	bool		m_isCooking = false;

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

	unsigned receiverId;

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

JE_END