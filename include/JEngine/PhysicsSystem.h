#pragma once
#include <vector>
#include "System.h"

jeBegin

class Transform;
class Rigidbody;

class PhysicsSystem : public System
{
	friend class SystemManager;

	using Transforms	= std::vector<Transform*>;
	using Rigidbodies	= std::vector<Rigidbody*>;

public:
	
private:

	PhysicsSystem();
	virtual ~PhysicsSystem() {};
	PhysicsSystem(const PhysicsSystem& /*copy*/) = delete;
	void operator=(const PhysicsSystem& /*copy*/) = delete;

	void Load(CR_RJDoc data) override;
	void Init() override;
	void Update(float dt) override;
	void Close() override;
	void Unload() override;

	void AddTransform(Transform* pTransform);
	void RemoveTransform(Transform* pTransform);

	void AddRigidbody(Transform* pRigidbody);
	void RemoveRigidbody(Transform* pRigidbody);
	
	Transforms		transforms_;
	Rigidbodies		rigidbodies_;
};

jeEnd
