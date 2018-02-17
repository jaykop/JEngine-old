#pragma once
#include <vector>
#include "System.h"

JE_BEGIN

class Object;
class CustomComponent;

class BehaviorSystem : public System
{
	friend class SystemManager;

	using Behaviors = std::vector<CustomComponent*> ;

public:

	void AddBehavior(CustomComponent* _behavior);
	void RemoveBehavior(CustomComponent* _behavior);

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
