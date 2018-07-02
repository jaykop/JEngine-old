#pragma once
#include <vector>
#include "Model.h"

jeBegin
jeDeclareComponentBuilder(Emitter);

class Emitter : public Model
{
    jeBaseFriends(Emitter);
    friend class GraphicSystem;

    enum ParticleType { PARTICLE_NORMAL, PARTICLE_EXPLODE, PARTICLE_WIDE, PARTICLE_SMOG };

    struct Particle {

        bool	hidden, dead;
        vec3	color, position, direction, velocity;
        float	life, rotationSpeed, rotation;
		Emitter	*pEmitter;
    };

    using Particles = std::vector<Particle*>;

public:

    void Register() override;

	void RefreshParticles();
    void SetQuantity(unsigned quantity);
    void SetColors(const vec3& start, const vec3& end);

	bool				active_;
    float				life_, rotationSpeed_;
    unsigned			size_;
    ParticleType		type_;
    vec3				direction_, velocity_, range_;

private:

    Emitter(Object* pOwner);
	virtual ~Emitter();
    void operator=(const Emitter& copy);

    void Load(CR_RJValue data) override;

	Particle* MakekParticle();
	void RefreshParticle(Particle* pParticle);

    Particles	particles_;
    vec3		startColor_, endColor_, colorDiff_;
    bool		changeColor_;
    unsigned	deadCount_;

    Emitter() = delete;
    Emitter(const Emitter& /*copy*/) = delete;

    void EditorUpdate(float dt) override;

    const static unsigned kMaxSize_;

};

jeEnd
