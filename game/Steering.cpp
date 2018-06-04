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

    return vec3();
}

vec3 Steering::Calculate()
{
    vec3 force;

    // case: Seek
    force = Seek(m_target->GetComponent<Transform>()->m_position) * 1.f/*m_dWeightSeek*/;

    if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;;
}

bool Steering::AccumulateForce(vec3& runningTot, const vec3& forceToAdd)
{
    float magnitudeSoFar = runningTot.GetLength();

    float magnitudeRemaining = maxForce - magnitudeSoFar;

    if (magnitudeRemaining < 0.f) return false;

    float magnitudeToAdd = forceToAdd.GetLength();

    if (magnitudeToAdd < magnitudeRemaining)
        runningTot += forceToAdd;

    else
        runningTot += forceToAdd.GetNormalize() * magnitudeRemaining;

    return true;
}

    Steering::Steering(Object* _pObject)
    :MovingEntity(_pObject)
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
    maxSpeed = 150.f;
}

void Truncate(float& value, float max)
{
    if (max < value)
        value = max;
}

void Steering::Update(const float _dt)
{
    heading = Seek(m_target->GetComponent<Transform>()->m_position);

    if (INPUT::KeyTriggered(JE_ENTER)) {
        m_target->GetComponent<Transform>()->m_position.Set(
            RAND::GetRandVec3(vec3(-75, -75, 0.f), vec3(75, 75, 0)));
    }
    
    vec3 acceleration = heading.GetNormalize();
    velocity += acceleration * _dt * maxSpeed;

    Truncate(velocity.x, maxSpeed);
    Truncate(velocity.y, maxSpeed);

    if (GetOwner()->HasComponent<Transform>()) {

        vec3 toAdd = velocity * _dt;
        toAdd.z = 0.f;
        GetOwner()->GetComponent<Transform>()->m_position += toAdd;
    }

    if (velocity.GetLengthSq() > 0.00000001) {
        heading = velocity.GetNormalize();
        side = heading.GetRotated(90, heading);
    }
}

void Steering::Close()
{}

bool Steering::OnMessage(Telegram& /*msg*/)
{
    return false;
}

jeDefineCustomComponentBuilder(Steering);

jeEnd