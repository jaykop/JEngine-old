#pragma once
#include <vector>
#include "System.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix4x4.h"

jeBegin

class Mesh;
class Font;
class Shader;
class Material;
class Transform;
class Animation;
struct Character;

enum ProjectType { PROJECTION_PERSPECTIVE, PROJECTION_ORTHOGONAL };

class GraphicSystem : public System
{
public:

	struct jeVertex {

		vec3 position;
		vec2 uv;
		vec3 normal;
	};

private:
	
    friend class Text;
    friend class Light;
    friend class Model;
    friend class Sprite;
    friend class Camera;
    friend class Emitter;
	friend class GLManager;
    friend class SystemManager;

	using Indices = std::vector<unsigned>;
	using Vertexes = std::vector<jeVertex>;
    using Lights = std::vector<Light*>;
    using Sprites = std::vector<Sprite*>;
    using Cameras = std::vector<Camera*>;

    enum ScreenEffect { EFFECT_NONE, EFFECT_BLUR, EFFECT_SOBEL, EFFECT_INVERSE };
    enum Alias { ALIAS_ALIASED, ALIAS_ANTIALIASED, ALIAS_MULTISAMPLE };

public:


    // TODO
    void    Ray(Sprite* _sprite, Transform* _transform);

    int	    GetWidth() const;
    int	    GetHeight() const;
    void    SetMainCamera(Camera* _camera);

    Camera* GetMainCamera() const;

    vec4			backgroundColor, screenColor;
    bool			orthoComesFirst;
    float			sobelAmount, blurSize, blurAmount, sobelSize, aspect, zNear, zFar;
    Alias			aliasMode;
    ScreenEffect    screenEffect;

private:

    GraphicSystem();
    ~GraphicSystem() {};

    void Load(CR_RJDoc _data) override;
    void Init() override;
    void Update(const float _dt) override;
    void Close() override;
    void Unload() override;

    // Helper functions
    void AddSprite(Sprite* _sprite);
    void RemoveSprite(Sprite* _sprite);

    void AddCamera(Camera* _camera);
    void RemoveCamera(Camera* _camera);

    void AddLight(Light* _light);
    void RemoveLight(Light* _light);

    void StartAntialiasing();
    void EndAntialiasing();

    void RenderToFramebuffer() const;
    void RenderToScreen() const;

    void UpdatePipelines(const float _dt);
    void LightSourcePipeline();
    void TextPipeline(Text * _text);
    void SpritePipeline(Sprite * _sprite);
    void ParentPipeline(Transform* _pTransform) const;
    void MappingPipeline(Sprite* _sprite);
    void LightingEffectPipeline(Material* _material);
    void ParticlePipeline(Emitter* _emitter, const float _dt);

	// New method
	void Render(const Mesh* _pMesh);
	void Render(unsigned _vao, unsigned _vbo, unsigned _ebo, 
		const Vertexes& _vertexes, const Indices& _indices, unsigned _drawMode);

    void Render(Font* _font, Text*_text, Transform* _transform, bool _printUnicode);
    void RenderCharacter(Character& _character, const vec3& _position,
        const vec3& _scale, float& _newX, float _intervalY);
    void SortSprites();

    // Member variables
    Lights	m_lights;
    Sprites	m_sprites;
    Cameras	m_cameras;
    Camera*	m_pMainCamera;

    vec3	m_resolutionScaler;

    unsigned	m_maxLights;

	Vertexes	m_vertexArray;

    int		m_width, m_height;
    bool	m_inside, m_isLight;
    mat4	m_perspective, m_orthogonal, m_viewport;
    vec3	m_aniScale, m_aniTranslate;
    float	m_left, m_right, m_top, m_bottom, m_mouseZ;
	
    GraphicSystem(const GraphicSystem& /*_copy*/) = delete;
    void operator=(const GraphicSystem& /*_copy*/) = delete;
};

jeEnd
