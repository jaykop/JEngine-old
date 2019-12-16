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

	using VertexIndices = std::vector<Mesh::jeIndex>;
	using Indices = std::vector<unsigned>;
    using Lights = std::vector<Light*>;
    using Models = std::vector<Model*>;
    using Cameras = std::vector<Camera*>;

    enum ScreenEffect { JE_EFFECT_NONE, JE_EFFECT_BLUR, JE_EFFECT_SOBEL, JE_EFFECT_INVERSE };
    enum Alias { JE_ALIAS_ALIASED, JE_ALIAS_ANTIALIASED, JE_ALIAS_MULTISAMPLE };

public:

    // TODO
    void    Ray(Model* _model, Transform* pTransform);

    int	    GetWidth() const;
    int	    GetHeight() const;
    void    SetMainCamera(Camera* pCamera);

    Camera* GetMainCamera() const;

    vec4			backgroundColor_, screenColor_;
    bool			orthoComesFirst_;
	float			sobelAmount_, blurSize_, blurAmount_, sobelSize_;
    Alias			aliasMode_;
    ScreenEffect    screenEffect_;

private:

    GraphicSystem();
	virtual ~GraphicSystem() {};

    void Load(CR_RJDoc data) override;
    void Init() override;
    void Update(float dt) override;
    void Close() override;
    void Unload() override;

    // Helper functions
    void AddModel(Model* pModel);
    void RemoveModel(Model* pMmodel);

    void AddCamera(Camera* pCamera);
    void RemoveCamera(Camera* pCamera);

    void AddLight(Light* pLight);
    void RemoveLight(Light* pLight);

    void StartAntialiasing();
    void EndAntialiasing();

    void RenderToFramebuffer() const;
    void RenderToScreen() const;

    void UpdatePipelines(float dt);
    void LightSourcePipeline();
    void TextPipeline(Text * pText);
    void ModelPipeline(Model * pModel);
    void ParentPipeline(Transform* pTransform) const;
    void MappingPipeline(Model* pModel);
    void LightingEffectPipeline(Material* pMaterial);
    void ParticlePipeline(Emitter* pEmitter, float dt);
	void SortModels();

	// Render functions
	void Render(const Mesh* pMesh, unsigned drawMode);
	void Render(const Text*pText);
    void RenderCharacter(Character& character, const vec3& position,
        const vec3& scale, float& newX, float intervalY);

    // Member variables
    Lights	lights_;
    Models	models_;
    Cameras	cameras_;
    Camera*	pMainCamera_;

    vec3		resolutionScaler_;
    unsigned	maxLights_;

    int		width_, height_;
    bool	inside_, isLight_;
    mat4	perspective_, orthogonal_, viewport_;
    vec3	aniScale_, aniTranslate_;
    float	left_, right_, top_, bottom_;
	
    GraphicSystem(const GraphicSystem& /*copy*/) = delete;
    void operator=(const GraphicSystem& /*copy*/) = delete;
};

jeEnd
