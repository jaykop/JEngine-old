#pragma once
#include <vector>
#include "System.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix4x4.h"
#include "Mesh.h"

jeBegin

class Font;
class Shader;
class Material;
class Transform;
class Animation;
struct Character;

enum ProjectType { PROJECTION_PERSPECTIVE, PROJECTION_ORTHOGONAL };

class GraphicSystem : public System
{
private:
	
    friend class Text;
    friend class Light;
    friend class Model;
    friend class Camera;
    friend class Emitter;
	friend class GLManager;
    friend class SystemManager;

	using VertexIndices = std::vector<Mesh::VertexIndex>;
	using Indices = std::vector<unsigned>;
    using Lights = std::vector<Light*>;
    using Models = std::vector<Model*>;
    using Cameras = std::vector<Camera*>;

    enum ScreenEffect { EFFECT_NONE, EFFECT_BLUR, EFFECT_SOBEL, EFFECT_INVERSE };
    enum Alias { ALIAS_ALIASED, ALIAS_ANTIALIASED, ALIAS_MULTISAMPLE };

public:

    // TODO
    void    Ray(Model* _model, Transform* _transform);

    int	    GetWidth() const;
    int	    GetHeight() const;
    void    SetMainCamera(Camera* _camera);

    Camera* GetMainCamera() const;

    vec4			backgroundColor, screenColor;
    bool			orthoComesFirst;
	float			sobelAmount, blurSize, blurAmount, sobelSize;
    Alias			aliasMode;
    ScreenEffect    screenEffect;

private:

    GraphicSystem();
    ~GraphicSystem() {};

    void Load(CR_RJDoc _data) override;
    void Init() override;
    void Update(float dt) override;
    void Close() override;
    void Unload() override;

    // Helper functions
    void AddModel(Model* _model);
    void RemoveModel(Model* _model);

    void AddCamera(Camera* _camera);
    void RemoveCamera(Camera* _camera);

    void AddLight(Light* _light);
    void RemoveLight(Light* _light);

    void StartAntialiasing();
    void EndAntialiasing();

    void RenderToFramebuffer() const;
    void RenderToScreen() const;

    void UpdatePipelines(float dt);
    void LightSourcePipeline();
    void TextPipeline(Text * _text);
    void ModelPipeline(Model * _model);
    void ParentPipeline(Transform* _pTransform) const;
    void MappingPipeline(Model* _model);
    void LightingEffectPipeline(Material* _material);
    void ParticlePipeline(Emitter* _emitter, float dt);
	void SortModels();

	// Render functions
	void Render(const Mesh* _pMesh);
	void Render(const Text*_pText);
	void Render(unsigned _vao, unsigned _indicesSize, unsigned _drawMode);
    void RenderCharacter(Character& _character, const vec3& _position,
        const vec3& _scale, float& _newX, float _intervalY);

    // Member variables
    Lights	m_lights;
    Models	m_models;
    Cameras	m_cameras;
    Camera*	m_pMainCamera;

    vec3	m_resolutionScaler;
    unsigned	m_maxLights;

    int		m_width, m_height;
    bool	m_inside, m_isLight;
    mat4	m_perspective, m_orthogonal, m_viewport;
    vec3	m_aniScale, m_aniTranslate;
    float	m_left, m_right, m_top, m_bottom;
	
    GraphicSystem(const GraphicSystem& /*_copy*/) = delete;
    void operator=(const GraphicSystem& /*_copy*/) = delete;
};

jeEnd
