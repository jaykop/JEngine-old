#pragma once
#include <vector>
#include "System.h"

jeBegin

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
	virtual ~BehaviorSystem() {};
	BehaviorSystem(const BehaviorSystem& /*copy*/) = delete;
	void operator=(const BehaviorSystem& /*copy*/) = delete;

	void Load(CR_RJDoc _data) override;
	void Init() override;
	void Update(float dt) override;
	void Close() override;
	void Unload() override;

	Behaviors m_behaviors;

};

jeEnd
