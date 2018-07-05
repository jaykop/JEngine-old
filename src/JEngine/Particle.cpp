#include "Particle.h"
#include "Transform.h"
#include "SystemManager.h"
#include "Random.h"
#include "Object.h"
#include "MathUtils.h"
#include "Mesh.h"
#include "AssetManager.h"

#ifdef  jeUseBuiltInAllocator
#include "MemoryAllocator.h"
#endif

jeBegin
jeDefineComponentBuilder(Emitter);

using namespace Math;

const unsigned	Emitter::kMaxSize_ = 1000;

Emitter::Emitter(Object* pOwner)
	:Model(pOwner), startColor_(vec3::ONE), changeColor_(true),
	endColor_(vec3::ZERO), life_(1.f), type_(PARTICLE_NORMAL), pointSize_(1.f),
	direction_(vec3::ZERO), velocity_(vec3::ZERO), active_(true), colorDiff_(vec3::ZERO),
	deadCount_(0), range_(vec3::ZERO), size_(0), rotationSpeed_(0.f)
{
	sfactor_ = GL_SRC_ALPHA;
	dfactor_ = GL_ONE;
	is_ |= IS_EMITTER;
}

Emitter::~Emitter() 
{
	// Clear particles
	for (auto particle : particles_) {
		delete particle;
		particle = nullptr;
	}

	SYSTEM::pGraphic_->RemoveModel(this);
}

void Emitter::operator=(const Emitter & /*copy*/)
{
	//startColor_.Set(copy.startColor_);
	//changeColor_ = copy.changeColor_;
	//endColor_.Set(copy.endColor_);
	//life = copy.life;
	//type = copy.type;
	//is2d = copy.is2d;
	//direction.Set(copy.direction);
	//velocity.Set(copy.velocity);
	//active = copy.active;
	//deadCount_ = copy.deadCount_;
	//pointSize = copy.pointSize;
	//range.Set(copy.range);
	//size = copy.size;
	//colorDiff_.Set(copy.colorDiff_);
	//rotationSpeed = copy.rotationSpeed;

	//SetQuantity(size);
	//auto copy = copy.particles_.begin();
	//for (auto particle = particles_.begin(); particle != particles_.end();
	//	particle++, copy++) 
	//	(*particle) = (*copy);
	//
}

void Emitter::Register()
{
	SYSTEM::pGraphic_->AddModel(this);
	if (GetOwner()->HasComponent<Transform>()) 
		pTransform_ = GetOwner()->GetComponent<Transform>();
}

void Emitter::RefreshParticles()
{
	deadCount_ = 0;
	for (auto particle : particles_)
		RefreshParticle(particle);
}

Emitter::Particle* Emitter::MakekParticle()
{
	Particle* newParticle = new Particle();

	Emitter* pEmitter = newParticle->pEmitter = this;
	newParticle->dead = false;

	static Transform* s_pTransform;
	s_pTransform = pEmitter->GetOwner()->GetComponent<Transform>();

	newParticle->life = Random::GetRandomFloat(0.f, pEmitter->life_);
	newParticle->velocity = Random::GetRandVec3(vec3::ZERO, pEmitter->velocity_);
	newParticle->position = s_pTransform->position_;
	newParticle->rotation = Random::GetRandomFloat(0.f, 360.f);
	newParticle->direction = Math::IsZero(pEmitter->direction_) ? Random::GetRandVec3(-vec3::ONE, vec3::ONE) : pEmitter->direction_;
	Normalize(newParticle->direction);
	newParticle->color.Set(pEmitter->startColor_);

	static float s_rotationSpeed = pEmitter->rotationSpeed_;

	newParticle->hidden = pEmitter->type_ == PARTICLE_EXPLODE ? false : true;

	if (s_rotationSpeed)
		newParticle->rotationSpeed = Random::GetRandomFloat(0., s_rotationSpeed);

	return newParticle;
}

void Emitter::RefreshParticle(Particle* pParticle)
{
	Emitter* pEmitter = pParticle->pEmitter;

	static Transform* s_pTransform;
	s_pTransform = pEmitter->GetOwner()->GetComponent<Transform>();

	pParticle->rotation = Random::GetRandomFloat(0.f, 360.f);
	pParticle->rotationSpeed = Random::GetRandomFloat(0., pEmitter->rotationSpeed_);

	pParticle->life = Random::GetRandomFloat(0.f, pEmitter->life_);
	pParticle->color.Set(pEmitter->startColor_);

	if (pEmitter->type_ == Emitter::PARTICLE_NORMAL) {

		pParticle->position = s_pTransform->position_;
		pParticle->hidden = false;
		pParticle->direction = Math::IsZero(pEmitter->direction_) ? Random::GetRandVec3(-vec3::ONE, vec3::ONE) : pEmitter->direction_;
	}

	else if (pEmitter->type_ == PARTICLE_EXPLODE) {

		// No more particle to update,
		// turn off the active toggle
		if (pEmitter->size_ == pEmitter->deadCount_)
			pEmitter->active_ = false;

		else if (!pParticle->dead) {

			// Ready for next update
			pParticle->position = s_pTransform->position_;
			pParticle->direction = Math::IsZero(pEmitter->direction_) ? Random::GetRandVec3(-vec3::ONE, vec3::ONE) : pEmitter->direction_;

			// Set dead and add number
			pParticle->dead = true;
			pParticle->hidden = true;
			pEmitter->deadCount_++;
		}
	}

	else if (pEmitter->type_ == Emitter::PARTICLE_WIDE) {

		pParticle->hidden = false;

		static vec3 s_position, s_range;
		s_range = pEmitter->range_;
		s_position = s_pTransform->position_;
		pParticle->direction  = Math::IsZero(pEmitter->direction_) ? Random::GetRandVec3(-vec3::ONE, vec3::ONE) : pEmitter->direction_;
		pParticle->position.x = Random::GetRandomFloat(s_position.x - s_range.x, s_position.x + s_range.x);
		pParticle->position.y = Random::GetRandomFloat(s_position.y - s_range.y, s_position.y + s_range.y);
		pParticle->position.z = Random::GetRandomFloat(s_position.z - s_range.z, s_position.z + s_range.z);

		pParticle->life = Random::GetRandomFloat(0.f, pEmitter->life_);
		pParticle->color.Set(pEmitter->startColor_);

	}
}

void Emitter::Load(CR_RJValue data)
{
	if (data.HasMember("Mesh")
		&& data["Mesh"].IsArray())
	{
		CR_RJValue loadedMeshes = data["Mesh"];

		for (unsigned meshIndex = 0; meshIndex < loadedMeshes.Size(); ++meshIndex) {

			CR_RJValue currentMesh = loadedMeshes[meshIndex];
			Mesh* newMesh = nullptr;

			// Check either if there is shape
			if (currentMesh.HasMember("Shape")
				&& currentMesh["Shape"].IsString()) {
				std::string meshType = currentMesh["Shape"].GetString();

				if (!strcmp(meshType.c_str(), "Point"))
					newMesh = Mesh::CreatePoint();

				else if (!strcmp(meshType.c_str(), "CrossRect"))
					newMesh = Mesh::CreateCrossRect();

				else if (!strcmp(meshType.c_str(), "Cube"))
					newMesh = Mesh::CreateCube();

				else if (!strcmp(meshType.c_str(), "Tetrahedron"))
					newMesh = Mesh::CreateTetrahedron();

				else if (!strcmp(meshType.c_str(), "Rect"))
					newMesh = Mesh::CreateRect();

				else 
					newMesh = ASSET::LoadObjFile(meshType.c_str());
			}
			// If not, set default mesh type
			else
				newMesh = Mesh::CreateRect();

			AddMesh(newMesh);

			// Load texture
			if (currentMesh.HasMember("Texture")
				&& currentMesh["Texture"].IsArray()) {
				for (unsigned textureIndex = 0; textureIndex < currentMesh["Texture"].Size(); ++textureIndex)
					newMesh->AddTexture(currentMesh["Texture"][textureIndex].GetString());
			}
		}
	}

	if (data.HasMember("DrawMode")
		&& data["DrawMode"].IsString()) {

		std::string drawType = data["DrawMode"].GetString();
		if (!strcmp(drawType.c_str(), "Triangles"))
			drawMode_ = GL_TRIANGLES;

		else if (!strcmp(drawType.c_str(), "Triangle_Strip"))
			drawMode_ = GL_TRIANGLE_STRIP;

		else if (!strcmp(drawType.c_str(), "Triangle_Fan"))
			drawMode_ = GL_TRIANGLE_FAN;

		else if (!strcmp(drawType.c_str(), "Lines"))
			drawMode_ = GL_LINES;

		else if (!strcmp(drawType.c_str(), "Line_Strip"))
			drawMode_ = GL_LINE_STRIP;

		else if (!strcmp("Quad", drawType.c_str()))
			drawMode_ = GL_QUADS;

		else if (!strcmp("Quad_Strip", drawType.c_str()))
			drawMode_ = GL_QUAD_STRIP;

		else if (!strcmp("Points", drawType.c_str()))
			drawMode_ = GL_POINTS;
	}

	if (data.HasMember("Flip")
		&& data["Flip"].GetBool())
		status_ |= IS_FLIPPED;

	if (data.HasMember("Projection")) {
		CR_RJValue loadedProjection = data["Projection"];

		if (!strcmp("Perspective", loadedProjection.GetString()))
			projection_ = PROJECTION_PERSPECTIVE;

		else if (!strcmp("Orthogonal", loadedProjection.GetString()))
			projection_ = PROJECTION_ORTHOGONAL;

		else
			jeDebugPrint("!Model - Wrong projection type: %s\n", loadedProjection.GetString());
	}

	if (data.HasMember("Bilboard")
		&& data["Bilboard"].GetBool())
		status_ |= IS_BILBOARD;

	if (data.HasMember("Active"))
		active_ = data["Active"].GetBool();

	if (data.HasMember("Life")) 
		life_ = data["Life"].GetFloat();

	if (data.HasMember("StartColor") 
		&& data.HasMember("EndColor")) {

		CR_RJValue loadedStartColor = data["StartColor"],
			loadedEndColor = data["EndColor"];

		startColor_.Set(loadedStartColor[0].GetFloat(),
			loadedStartColor[1].GetFloat(),
			loadedStartColor[2].GetFloat());

		endColor_.Set(loadedEndColor[0].GetFloat(),
			loadedEndColor[1].GetFloat(),
			loadedEndColor[2].GetFloat());

		SetColors(startColor_, endColor_);
	}

	if (data.HasMember("Type")) {
		CR_RJValue loadedType = data["Type"];
		
		if (!strcmp(loadedType.GetString(), "Normal"))
			type_ = PARTICLE_NORMAL;
		else if (!strcmp(loadedType.GetString(), "Wide"))
			type_ = PARTICLE_WIDE;
		else if (!strcmp(loadedType.GetString(), "Explosion"))
			type_ = PARTICLE_EXPLODE;
	}

	if (data.HasMember("Range")) {
		CR_RJValue loadedRange = data["Range"];
		range_.Set(loadedRange[0].GetFloat(),
			loadedRange[1].GetFloat(),
			loadedRange[2].GetFloat());
	}

	if (data.HasMember("Direction")) {
		CR_RJValue loadedDirection = data["Direction"];
		direction_.Set(loadedDirection[0].GetFloat(),
			loadedDirection[1].GetFloat(),
			loadedDirection[2].GetFloat());
	}
	
	if (data.HasMember("Velocity")) {
		CR_RJValue loadedVelocity = data["Velocity"];
		velocity_.Set(loadedVelocity[0].GetFloat(),
			loadedVelocity[1].GetFloat(),
			loadedVelocity[2].GetFloat());
	}

	if (data.HasMember("Quantity"))
		SetQuantity(data["Quantity"].GetUint());

	if (data.HasMember("RotationSpeed"))
		rotationSpeed_ = data["RotationSpeed"].GetFloat();

	if (data.HasMember("PointSize"))
		pointSize_ = data["PointSize"].GetFloat();
}

void Emitter::SetQuantity(unsigned quantity)
{
	if (particles_.empty()) {

		if (kMaxSize_ < quantity) {
			quantity = kMaxSize_;
			jeDebugPrint("!Emitter - The quantity of particle must be less than 1000.\n");
		}

		for (unsigned i = 0; i < quantity; ++i)
			particles_.push_back(MakekParticle());
		size_ = quantity;
	}

	else
		jeDebugPrint("!Emitter - Already allocated.\n");
}

void Emitter::SetColors(const vec3& start, const vec3& end)
{
	startColor_ = start, endColor_ = end;
	colorDiff_ = (endColor_ - startColor_) / life_;

	// If the idff is zero, no need to add diff
	if (colorDiff_ == vec3::ZERO)
		changeColor_ = false;

	else
		changeColor_ = true;
}

void Emitter::EditorUpdate(const float /*dt*/)
{
	// TODO
}

jeEnd
