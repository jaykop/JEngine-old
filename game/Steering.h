#pragma once

#include "CustomComponent.h"
#include "ComponentManager.h"
#include "ComponentBuilder.h"
#include "Vector3.h"

jeBegin;

class Object;
class Transform;

class Steering : public CustomComponent
{
    jeBaseFriends(Steering);

	enum behavior{ none, seek, flee, arrive, pursuit, 
		evade, wander, obstacle_avoidance,  };

public:

    vec3 Seek(const vec3& _targetPos);
	vec3 Flee(const vec3& _targetPos);
	vec3 Arrive(const vec3& _targetPos);
	vec3 Evade(const Steering * _pursuer);
	vec3 Pursuit(const Steering * _evader);
	vec3 Wander();
	vec3 Avoid();

    bool AccumulateForce(const vec3& _forceToAdd);
    vec3 Calculate();
	vec3 GetPosOriginatedToDitection(Transform* _transform);
	vec3 GetWorldDetection();
	void GetIntersectedObstalces();
	
    Object* m_target = nullptr, *m_circle= nullptr, *m_detection = nullptr;

    vec3 velocity;
    vec3 heading;
    vec3 side;
	
	vec3 steeringForce;
	vec3 wanderTarget;
	vec3 leftTop, leftBot, rightTop, rightBot;

    float mass;
    float maxSpeed;
    float maxForce;
	float deceleration;

	float wanderRadius;
	float wanderJitter;
	float wanderDistance;
	
	float zPos;

	Steering	*m_evader, *m_pursuer;
	Transform	*m_transform, *targetTransform, *circleTransform, *detectionTransform;
	behavior	m_behavior;

	std::vector<Object*> m_obstacles, m_intersected;

private:

    Steering(Object* pObject);
    ~Steering() {};

    void Register() override;
    void Load(CR_RJValue _data) override;
    void Init() override;
    void Update(const float _dt) override;
    void Close() override;
    void Unload() override {};
    bool OnMessage(Telegram& msg) override;

    void EditorUpdate(const float /*_dt*/) override {};
};

jeDeclareCustomComponentBuilder(Steering);

jeEnd;