#pragma once
#include <vector>
#include "System.h"

JE_BEGIN

class Object;
class UserComponent;

class BehaviorSystem : public System
{
	friend class SystemManager;

	using Behaviors = std::vector<UserComponent*> ;

public:

	void AddBehavior(UserComponent* _behavior);
	void RemoveBehavior(UserComponent* _behavior);

private:

	BehaviorSystem();
	~BehaviorSystem() {};
	BehaviorSystem(const BehaviorSystem& /*_copy*/) = delete;
	void operator=(const BehaviorSystem& /*_copy*/) = delete;

	void Load(CR_RJDoc _data) override;
	void Init() override;
	void Update(const float _dt) override;
	void Close() override;
	void Unload() override;

	Behaviors m_behaviors;

};

JE_END
