#include "Steering.h"
#include "CustomLogicHeader.h"
#include "Random.h"

jeBegin

using namespace Math;

// Helper functions
void Truncate(float& value, float max)
{
	if (max < value)
		value = max;
}

// Steering functions body
vec3 Steering::Seek(const vec3& _targetPos)
{
	if (m_transform) {
		Transform *transform = GetOwner()->GetComponent<Transform>();
		vec3 desiredVel = (_targetPos - transform->position) * maxSpeed;

		return desiredVel - velocity;
	}

	return vec3::ZERO;
}

vec3 Steering::Flee(const vec3& _targetPos)
{
	if (GetOwner()->HasComponent<Transform>()) {
		Transform *transform = GetOwner()->GetComponent<Transform>();
		vec3 desiredVel = (transform->position - _targetPos) * maxSpeed;

		return desiredVel - velocity;
	}
	return vec3::ZERO;
}

vec3 Steering::Arrive(const vec3& _targetPos)
{
	vec3 toTarget = _targetPos - m_transform->position;

	float distance = GetLength(toTarget);

	if (distance > 0) {

		//
		static const float decelerationTweaker = 0.3f;

		// Get new speed and modify it
		float s_speed = distance / (deceleration * decelerationTweaker);
		Truncate(s_speed, maxSpeed);

		// 
		vec3 desiredVelocity = toTarget * s_speed / distance;

		return (desiredVelocity - velocity);
	}

	return vec3::ZERO;
}

vec3 Steering::Evade(const Steering* _pursuer)
{
	vec3 toPursuer = _pursuer->m_transform->position - m_transform->position;

	float lookAheadTime = GetLength(toPursuer) / (maxSpeed + GetLength(_pursuer->velocity));

	// now seek to the pridicted future position of the evader
	return Flee(_pursuer->m_transform->position + _pursuer->velocity * lookAheadTime);
}

vec3 Steering::Pursuit(const Steering* _evader)
{
	// If the evader is ahead and facing the agent then we can just seek
	// for the evader's current position
	vec3 toEvader = _evader->m_transform->position - m_transform->position;

	float relativeHeading = DotProduct(heading, _evader->heading);

	if (DotProduct(toEvader, heading) > 0
		&& (relativeHeading < -0.95)) // acos(0.95)=18 degs
		return Seek(_evader->m_transform->position);

	// Otherwise, this object predict where the evader will be.

	// the look-ahead time is propotional to the distanc between the evader
	// and the pursuer - and is inversly porpotional to the sum of the agent's velocities
	float lookAheadTime = GetLength(toEvader) / (maxSpeed + GetLength(_evader->velocity));

	// now seek to the pridicted future position of the evader
	return Seek(_evader->m_transform->position + _evader->velocity * lookAheadTime);
}

vec3 Steering::Wander()
{
	// Get the random direction from current wander target position

	// Get a direction to go next frame first
	// Get random degree
	static float randomDegree = RAND::GetRandomFloat(0, 360.F);
	static const float room = 45.f;
	static const float wanderForce = 100.f;
	// Get near random degree from last one
	randomDegree = RAND::GetRandomFloat(randomDegree - room, randomDegree + room);
	float randomRadian = Math::DegToRad(randomDegree);
	// Get random point on the circle
	vec3 nextPointToGo;
	nextPointToGo.Set(cosf(randomRadian), sinf(randomRadian), 0.f);
	Normalize(nextPointToGo);
	nextPointToGo *= wanderRadius;

	// Set the wandar center
	vec3 wanderCenter;
	wanderCenter.Set(m_transform->position);
	wanderCenter += heading * (m_transform->scale.x / 2.f + wanderRadius);
	wanderCenter.z = zPos;
	// Put the circle on the correct position
	circleTransform->position.Set(wanderCenter);
	// Set the wander target position
	wanderTarget.Set(
		wanderCenter.x + nextPointToGo.x, wanderCenter.y + nextPointToGo.y, zPos);
	// Put the haircross on the correct position 
	targetTransform->position.Set(wanderTarget);

	// Return the velocity
	return GetNormalize(wanderTarget - m_transform->position) * wanderForce;
}

vec3 Steering::Calculate()
{
	// Refresh the force
	steeringForce.SetZero();

	// Force to add to steering force
	vec3 force;

	// case: Seek
	switch (m_behavior) {
	case seek:
		force = Seek(targetTransform->position);
		break;

	case flee:
		force = Flee(targetTransform->position);
		break;

	case arrive:
		force = Arrive(targetTransform->position);
		break;

	case pursuit:
		force = Pursuit(m_evader);
		break;

	case evade:
		force = Evade(m_pursuer);
		break;

	case wander:
		force = Wander();
		break;

	default:
		break;
	}

	if (!AccumulateForce(force))
		return steeringForce;

	return steeringForce;
}

bool Steering::AccumulateForce(const vec3& forceToAdd)
{
	// Get the current magnitude
	float magnitudeSoFar = GetLength(steeringForce);

	// Get the remaining magnitude
	float magnitudeRemaining = maxForce - magnitudeSoFar;

	// If there is no more remaining magnitude to add,
	// return false
	if (magnitudeRemaining <= 0.f) return false;

	// Otherwise, get force to add
	float magnitudeToAdd = GetLength(forceToAdd);

	// If that valuse is lower than capable remaining magnitude,
	// just add it
	if (magnitudeToAdd < magnitudeRemaining)
		steeringForce += forceToAdd;

	// If not, modify its length and add it
	else
		steeringForce += GetNormalize(forceToAdd) * maxForce;

	return true;
}

jeEnd