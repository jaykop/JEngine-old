#pragma once
#include <vector>
#include "System.h"

JE_BEGIN

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
	~PhysicsSystem() {};
	PhysicsSystem(const PhysicsSystem& /*_copy*/) = delete;
	void operator=(const PhysicsSystem& /*_copy*/) = delete;

	void Load(CR_RJDoc _data) override;
	void Init() override;
	void Update(const float _dt) override;
	void Close() override;
	void Unload() override;

	void AddTransform(Transform* _transform);
	void RemoveTransform(Transform* _transform);

	void AddRigidbody(Transform* _rigidbody);
	void RemoveRigidbody(Transform* _rigidbody);

	void UpdateEdges(Transform* _transform);

	Transforms		m_transforms;
	Rigidbodies		m_rigidboide;
};

JE_END
