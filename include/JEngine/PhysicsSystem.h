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
	PhysicsSystem(const PhysicsSystem& /*_copy*/) = delete;
	void operator=(const PhysicsSystem& /*_copy*/) = delete;

	void Load(CR_RJDoc _data) override;
	void Init() override;
	void Update(float dt) override;
	void Close() override;
	void Unload() override;

	void AddTransform(Transform* _transform);
	void RemoveTransform(Transform* _transform);

	void AddRigidbody(Transform* _rigidbody);
	void RemoveRigidbody(Transform* _rigidbody);
	
	Transforms		m_transforms;
	Rigidbodies		m_rigidboide;
};

jeEnd
