#pragma once
#include <vector>
#include "System.h"

JE_BEGIN

class Object;
class UserComponent;

class BehaviorSystem : public System
{
	friend class UserComponent;
	friend class SystemManager;

	typedef std::vector<UserComponent*> Behaviors;

public:

	void AddBehavior(UserComponent* _behavior);
	void RemoveBehavior(UserComponent* _behavior);

private:

	BehaviorSystem();
	~BehaviorSystem() {};
	BehaviorSystem(const BehaviorSystem& /*_copy*/) {};
	void operator=(const BehaviorSystem& /*_copy*/) {};

	void Load(CR_RJDoc _data) override;
	void Init() override;
	void Update(float dt) override;
	void Close() override;
	void Unload() override;

	Behaviors m_behaviors;

};

JE_END
