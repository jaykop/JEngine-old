#include "PhysicsSystem.h"
#include "PhysicsComponents.h"

JE_BEGIN

PhysicsSystem::PhysicsSystem()
	:System()
{
}

void PhysicsSystem::Load(CR_RJDoc /*_data*/)
{
}

void PhysicsSystem::Init()
{
}

void PhysicsSystem::Update(const float /*_dt*/)
{
	// TODO
	//for (auto transform : m_transforms)
	//	UpdateEdges(transform);
}

void PhysicsSystem::Close()
{
}

void PhysicsSystem::Unload()
{
}

void PhysicsSystem::UpdateEdges(Transform *_transform)
{
	vec3 position = _transform->m_position;
	vec3 halfScale = _transform->m_scale * .5f;
	vec3 temp = position - halfScale;

	_transform->m_edges.m_leftBot = temp;

	temp.x = -temp.x;
	_transform->m_edges.m_rightBot = temp;

	temp.y = -temp.y;
	_transform->m_edges.m_rightTop = temp;
		
	temp.x = -temp.x;
	_transform->m_edges.m_leftTop = temp;

}

JE_END
