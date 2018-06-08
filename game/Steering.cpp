#include "Steering.h"
#include "CustomLogicHeader.h"
#include "Random.h"

jeBegin

// Helper functions

void Truncate(vec3& vector, float _max)
{
	if (vector.GetLength() > _max) {
		vector.Normalize();
		vector *= _max;
	}
}

void ControlPosition(vec3& _position)
{
	static const float halfWidth = SYSTEM::GetGraphicSystem()->GetWidth() / 2.f;
	static const float halfHeight = SYSTEM::GetGraphicSystem()->GetHeight() / 2.f;

	if (_position.x > halfWidth)
		_position.x = -halfWidth;

	else if (_position.x < -halfWidth)
		_position.x = halfWidth;

	if (_position.y > halfHeight)
		_position.y = -halfHeight;

	else if (_position.y < -halfHeight)
		_position.y = halfHeight;

}

Steering::Steering(Object* _pObject)
    :CustomComponent(_pObject)
{}

void Steering::Register()
{
    SYSTEM::GetBehaviorSystem()->AddBehavior(this);
}

void Steering::Load(CR_RJValue _data)
{
	if (!strcmp(_data["Behavior"].GetString(), "Seek"))
		m_behavior = seek;
	else if (!strcmp(_data["Behavior"].GetString(), "Flee"))
		m_behavior = flee;
	else if (!strcmp(_data["Behavior"].GetString(), "Arrive"))
		m_behavior = arrive;
	else if (!strcmp(_data["Behavior"].GetString(), "Pursuit"))
		m_behavior = pursuit;
	else if (!strcmp(_data["Behavior"].GetString(), "Evade"))
		m_behavior = evade;
	else if (!strcmp(_data["Behavior"].GetString(), "Wander"))
		m_behavior = wander;
}

void Steering::Init()
{
	
	// Setting for each behavior mode
	if (m_behavior == pursuit)
		m_evader = CONTAINER->GetObject("Evader")->GetComponent<Steering>();
	
	else if (m_behavior == evade)
		m_pursuer = CONTAINER->GetObject("Pursuer")->GetComponent<Steering>();
	
	else if (m_behavior == wander)
	{
		m_circle = CONTAINER->GetObject("Circle");
		circleTransform = m_circle->GetComponent<Transform>();
		wanderRadius = circleTransform->m_scale.x / 2.f;
	}

	// Get target transform
    m_target = CONTAINER->GetObject("Target");
	targetTransform = m_target->GetComponent<Transform>();
	
	// Get owner's transform
	m_transform = GetOwner()->GetComponent<Transform>();
	zPos = m_transform->m_position.z;

	mass = 1.f;
    maxSpeed = 50.f;
	maxForce = 250.f;

	deceleration = 2.f;

	wanderTarget.SetZero();
	velocity.SetZero();
}

vec3 GetPerpendicular(const vec3& vector)
{
	vec3 result;
	result.x = -vector.y;
	result.y = vector.x;

	return result;
}

void Steering::Update(const float _dt)
{
	// By mouse
	if (INPUT::KeyTriggered(JE_MOUSE_LEFT))
		targetTransform->m_position.Set(INPUT::GetOrhtoPosition());
	
	// By keyboard
	if (INPUT::KeyTriggered(JE_ENTER))
		targetTransform->m_position.Set(
			Random::GetRandVec3(-350.f, -250.f, 0.f, 350.f, 250.f, 0.f));

	// Calculate the force to add
	Calculate();

	// Add force to velocity
	vec3 acceleration = steeringForce.GetNormalize() / mass;
	velocity += acceleration * _dt * maxSpeed;

	// Limit the velocity magnitude
	Truncate(velocity, maxSpeed);

	// Update position
	vec3 toAdd = velocity * _dt;
	toAdd.z = 0.f;
	m_transform->m_position += toAdd;
	m_transform->m_rotation = vec3::UNIT_X.GetAngle(velocity);

	if (velocity.GetLengthSq() > 0.00000001) {
		heading = velocity.GetNormalize();
		side = GetPerpendicular(heading);
	}

	ControlPosition(m_transform->m_position);
}

void Steering::Close()
{}

bool Steering::OnMessage(Telegram& /*msg*/)
{
    return false;
}

jeDefineCustomComponentBuilder(Steering);

jeEnd