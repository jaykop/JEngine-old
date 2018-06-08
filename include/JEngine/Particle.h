#pragma once
#include <vector>
#include "Sprite.h"
#include "Vector3.h"

jeBegin
jeDeclareComponentBuilder(Emitter);

class Emitter : public Sprite
{
    jeBaseFriends(Emitter);
    friend class GraphicSystem;

    enum ParticleType { PARTICLE_NORMAL, PARTICLE_EXPLODE, PARTICLE_WIDE, PARTICLE_SMOG };
    enum ParticleRendertType { PARTICLERENDER_POINT, PARTICLERENDER_PLANE, PARTICLERENDER_3D };

    class Particle {

        friend class GraphicSystem;
        friend class Emitter;

    public:

        void	Refresh();

        bool	hidden, dead;
        vec3	color, position, direction, velocity;
        float	life, rotationSpeed, rotation;

    private:

        Particle(Emitter* _emitter);
        ~Particle() {};
        void operator=(const Particle& _copy);

        Emitter* m_pEmitter;

        Particle() = delete;
        Particle(const Particle& /*_copy*/) = delete;

    };

    using Particles = std::vector<Particle*>;

public:

    void Register() override;

    void ManualRefresh();
    void SetQuantity(unsigned _quantity);
    void SetColors(const vec3& _start, const vec3& _end);

    bool				active, is2d;
    float				life, rotationSpeed, pointSize;
    unsigned			size;
    ParticleType		type;
    ParticleRendertType	renderType;
    vec3				direction, velocity, range;

private:

    Emitter(Object* _pOwner);
    ~Emitter();
    void operator=(const Emitter& _copy);

    void Load(CR_RJValue _data) override;
    void Refresh(Particle* _particle);

    Particles	m_particles;
    vec3		m_startColor, m_endColor, colorDiff;
    bool		m_changeColor;
    unsigned	m_deadCount;

    Emitter() = delete;
    Emitter(const Emitter& /*_copy*/) = delete;

    void EditorUpdate(const float _dt) override;

    const static unsigned m_maxSize;

};

jeEnd
