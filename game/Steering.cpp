#include "Steering.h"
#include "CustomLogicHeader.h"
#include "Random.h"

jeBegin

vec3 Steering::Seek(const vec3& _targetPos)
{
    if (GetOwner()->HasComponent<Transform>()) {
        Transform *transform = GetOwner()->GetComponent<Transform>();
        vec3 desiredVel = (_targetPos - transform->m_position) * maxSpeed;

        return desiredVel - velocity;
    }

    return vec3::ZERO;
}

vec3 Steering::Flee(const vec3& _targetPos)
{
	if (GetOwner()->HasComponent<Transform>()) {
		Transform *transform = GetOwner()->GetComponent<Transform>();
		vec3 desiredVel = (transform->m_position - _targetPos) * maxSpeed;

		return desiredVel - velocity;
	}
	return vec3::ZERO;
}

vec3 Steering::Calculate()
{
    vec3 force;

    // case: Seek
    force = Seek(m_target->GetComponent<Transform>()->m_position) /** 1.f*/;

    if (!AccumulateForce(force)) 
		return steeringForce;

	return steeringForce;
}

bool Steering::AccumulateForce(const vec3& forceToAdd)
{
	// Get the current magnitude
    float magnitudeSoFar = steeringForce.GetLength();

	// Get the remaining magnitude
    float magnitudeRemaining = maxForce - magnitudeSoFar;

	// If there is no more remaining magnitude to add,
	// return false
    if (magnitudeRemaining <= 0.f) return false;

	// Otherwise, get force to add
    float magnitudeToAdd = forceToAdd.GetLength();

	// If that valuse is lower than capable remaining magnitude,
	// just add it
    if (magnitudeToAdd < magnitudeRemaining)
		steeringForce = forceToAdd;

	// If not, modify its length and add it
    else
		steeringForce = forceToAdd.GetNormalize() * maxForce;

    return true;
}

    Steering::Steering(Object* _pObject)
    :CustomComponent(_pObject)
{}

void Steering::Register()
{
    SYSTEM::GetBehaviorSystem()->AddBehavior(this);
}

void Steering::Load(CR_RJValue /*_data*/)
{}

void Steering::Init()
{
    m_target = CONTAINER->GetObject("Target");
    mass = 1.f;
    maxSpeed = 50.f;
	maxForce = 250.f;
	velocity.SetZero();
}

void Truncate(float& value, float max)
{
    if (max < value)
        value = max;
}

void ControlPosition(vec3& _position)
{
	const float halfWidth = SYSTEM::GetGraphicSystem()->GetWidth() / 2.f;
	const float halfHeight = SYSTEM::GetGraphicSystem()->GetHeight() / 2.f;
	
	if (_position.x > halfWidth)
		_position.x = -halfWidth;

	else if (_position.x < -halfWidth)
		_position.x = halfWidth;

	if (_position.y > halfHeight)
		_position.y = -halfHeight;

	else if (_position.y < -halfHeight)
		_position.y = halfHeight;

}

void Steering::Update(const float _dt)
{
	Transform* targetTransform = nullptr;
	if (m_target->GetComponent<Transform>())
		targetTransform = m_target->GetComponent<Transform>();

	if (INPUT::KeyTriggered(JE_MOUSE_LEFT))
		m_target->GetComponent<Transform>()->m_position.Set(INPUT::GetOrhtoPosition());

	if (GetOwner()->HasComponent<Transform>())	{
		Transform* transform = GetOwner()->GetComponent<Transform>();

		velocity += Flee(targetTransform->m_position).GetNormalize() * _dt * maxForce;

		velocity.Truncate(maxSpeed);
		velocity.z = 0.f;

		transform->m_rotation = vec3::UNIT_X.GetAngle(velocity);
		transform->m_position += velocity * _dt;

		ControlPosition(transform->m_position);
	}

	//Calculate();

	//vec3 acceleration = steeringForce.GetNormalize() / mass;

	//velocity += acceleration * _dt;

	//velocity.Truncate(maxSpeed);

	//// Update position
	//if (GetOwner()->HasComponent<Transform>()) {

	//	vec3 toAdd = velocity * _dt;
	//	toAdd.z = 0.f;
	//	GetOwner()->GetComponent<Transform>()->m_position += toAdd;
	//}

	//if (velocity.GetLengthSq() > 0.00000001) {
	//	heading = velocity.GetNormalize();
	//	side = heading.Perpendicular();
	//}
}

void Steering::Close()
{}

bool Steering::OnMessage(Telegram& /*msg*/)
{
    return false;
}

jeDefineCustomComponentBuilder(Steering);

jeEnd