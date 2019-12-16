#include "Steering.h"
#include "CustomLogicHeader.h"
#include "Random.h"

jeBegin

using namespace Math;

// Helper functions

void Truncate(vec3& vector, float _max)
{
	if (GetLength(vector) > _max) {
		Normalize(vector);
		vector *= _max;
	}
}

void ControlPosition(vec3& _position)
{
	static const float halfWidth = SYSTEM::pGraphic_->GetWidth() / 2.f;
	static const float halfHeight = SYSTEM::pGraphic_->GetHeight() / 2.f;

	if (_position.x > halfWidth)
		_position.x = -halfWidth;

	else if (_position.x < -halfWidth)
		_position.x = halfWidth;

	if (_position.y > halfHeight)
		_position.y = -halfHeight;

	else if (_position.y < -halfHeight)
		_position.y = halfHeight;

}

Steering::Steering(Object* pObject)
	:CustomComponent(pObject)
{}

void Steering::Register()
{
	SYSTEM::pBehavior_->AddBehavior(this);
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
	else if (!strcmp(_data["Behavior"].GetString(), "Avoid"))
		m_behavior = obstacle_avoidance;
}

void Steering::Init()
{
	// Get owner's transform
	m_transform = GetOwner()->GetComponent<Transform>();
	zPos = m_transform->position_.z;

	mass = 1.f;
	maxSpeed = 50.f;
	maxForce = 250.f;

	deceleration = 2.f;

	wanderTarget.SetZero();
	velocity.SetZero();

	// Setting for each behavior mode
	if (m_behavior == pursuit)
		m_evader = OBJECT::pContainer_->GetObject("Seeker")->GetComponent<Steering>();

	else if (m_behavior == evade)
		m_pursuer = OBJECT::pContainer_->GetObject("Pursuer")->GetComponent<Steering>();

	else if (m_behavior == wander)
	{
		m_circle = OBJECT::pContainer_->GetObject("Circle");
		circleTransform = m_circle->GetComponent<Transform>();
		wanderRadius = circleTransform->scale_.x / 2.f;
	}

	else if (m_behavior == obstacle_avoidance) {
		
		m_detection = OBJECT::pContainer_->GetObject("PathBox");
		
		detectionTransform = m_detection->GetComponent<Transform>();
		detectionTransform->scale_.x = 5.f;
		detectionTransform->scale_.y = 1.f;
		detectionTransform->position_.x += detectionTransform->scale_.x / 2.f;

		Model *pathBoxModel = m_detection->GetComponent<Model>();
		pathBoxModel->SetParentToFollow(GetOwner());
		pathBoxModel->projection_ = PROJECTION_PERSPECTIVE;

		for (int index = 0; index < 5; ++index)	{
			FACTORY::CreateObject(("Obstacle " + std::to_string(index)).c_str());
			Object* newObstacle = FACTORY::GetCreatedObject();
			
			newObstacle->AddComponent<Transform>();
			Transform* transform = newObstacle->GetComponent<Transform>();
			transform->position_.Set(RAND::GetRandVec3(-50.f, -50.f, -1.f, 50.f, 50.f, -1.f));
			float randomScale = RAND::GetRandomFloat(5.f, 15.f);
			transform->scale_.Set(randomScale, randomScale, 0.f);

			newObstacle->AddComponent<Model>();
			Model* model = newObstacle->GetComponent<Model>();
			model->color_.Set(1.f, 0.f, 0.f, 1.f);
			Mesh* newMesh = Mesh::CreateRect();
			model->AddMesh(newMesh);
			newMesh->AddTexture("circle");
			model->projection_ = PROJECTION_PERSPECTIVE;

			FACTORY::AddCreatedObject();

			m_obstacles.push_back(newObstacle);
		}
	}

	// Get target transform
	m_target = OBJECT::pContainer_->GetObject("Target");
	targetTransform = m_target->GetComponent<Transform>();

}

vec3 GetPerpendicular(const vec3& vector)
{
	vec3 result;
	result.x = -vector.y;
	result.y = vector.x;

	return result;
}

void Steering::Update(float dt)
{
	// By mouse
	if (INPUT::KeyTriggered(JE_MOUSE_LEFT)) {
		vec3 newPos(INPUT::GetOrhtoPosition().x, INPUT::GetOrhtoPosition().y, -1.f);
		targetTransform->position_.Set(newPos);
	}

	if (INPUT::KeyTriggered(JE_SPACE))
	{
		static behavior save = none;
		
		behavior temp = m_behavior;
		m_behavior = save;
		save = temp;
	}

	if (m_detection)
		m_detection->GetComponent<Transform>()->position_.z = -1.f;

	// By keyboard
	if (INPUT::KeyTriggered(JE_ENTER))
		targetTransform->position_.Set(
			Random::GetRandVec3(-350.f, -250.f, -1.f, 350.f, 250.f, -1.f));

	if (m_behavior != none) {

		Avoid();

		// Calculate the force to add
		Calculate();

		// Add force to velocity
		vec3 acceleration = GetNormalize(steeringForce) / mass;
		velocity += acceleration * dt * maxSpeed;

		// Limit the velocity magnitude
		Truncate(velocity, maxSpeed);

		// Update position
		vec3 toAdd = velocity * dt;
		toAdd.z = 0.f;
		m_transform->position_ += toAdd;
		m_transform->rotation_ = GetAngle(vec3::UNIT_X, velocity);

		if (GetLengthSq(velocity) > 0.00000001) {
			heading = GetNormalize(velocity);
			side = GetPerpendicular(heading);
		}

		ControlPosition(m_transform->position_);
	}
}

void Steering::Close()
{
	m_obstacles.clear();
}

bool Steering::OnMessage(Telegram& /*msg*/)
{
	return false;
}

jeDefineCustomComponentBuilder(Steering);

jeEnd