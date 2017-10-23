#pragma once
#include <vector>
#include "System.h"

JE_BEGIN

class Transform;
class Rigidbody;
class PhysicsSystem : public System
{
	friend class SystemManager;

	typedef std::vector<Transform*> Transforms;
	typedef std::vector<Rigidbody*> Rigidbodies;

public:
	
private:

	PhysicsSystem();
	~PhysicsSystem() {};
	PhysicsSystem(const PhysicsSystem& /*_copy*/) {};
	void operator=(const PhysicsSystem& /*_copy*/) {};

	void Load() override;
	void Init() override;
	void Update(float dt) override;
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
