#pragma once
#include <vector>
#include "System.h"
#include "Matrix4x4.h"

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
    friend class Text;
    friend class Light;
    friend class Model;
    friend class Sprite;
    friend class Camera;
    friend class Emitter;
    friend class SystemManager;

    using Lights = std::vector<Light*>;
    using Sprites = std::vector<Sprite*>;
    using Cameras = std::vector<Camera*>;

    enum ScreenEffect { EFFECT_NONE, EFFECT_BLUR, EFFECT_SOBEL, EFFECT_INVERSE };
    enum Alias { ALIAS_ALIASED, ALIAS_ANTIALIASED, ALIAS_MULTISAMPLE };

public:

    // TODO
    void	Ray(Sprite* _sprite, Transform* _transform);

    int		GetWidth() const;
    int		GetHeight() const;
    void	SetMainCamera(Camera* _camera);
    Camera*     GetMainCamera();

    vec3	    m_lightScale;
    vec4	    m_backgroundColor, m_screenColor;
    bool	    m_orthoComesFirst;
    float	    m_sobelAmount, m_blurSize, m_blurAmount, m_sobelSize, m_fovy, m_aspect, m_zNear, m_zFar;
    Alias	    m_aliasMode;
    ScreenEffect    m_screenEffect;

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

    void RenderToFramebuffer();
    void RenderToScreen();

    void UpdatePipelines(const float _dt);
    void LightSourcePipeline();
    void TextPipeline(Text * _text);
    void SpritePipeline(Sprite * _sprite);
    void MappingPipeline(Sprite* _sprite);
    void LightingEffectPipeline(Material* _material);
    void ParticlePipeline(Emitter* _emitter, const float _dt);

    void Render(const unsigned &_vao, const int _elementSize);
    void Render(Font* _font, Text*_text, Transform* _transform, bool _printUnicode);
    void RenderCharacter(Character& _character, const vec3& _position,
        const vec3& _scale, float& _newX, float _intervalY);
    void SortSprites();
    void UpdateMousePosition();

    // Member variables
    Lights		m_lights;
    Sprites		m_sprites;
    Cameras		m_cameras;
    Camera*		m_pMainCamera;

    vec3		m_resolutionScaler;

    unsigned m_maxLights;

    int		m_width, m_height;
    bool	m_inside, m_isLight;
    mat4	m_perspective, m_orthogonal, m_viewport;
    vec3	m_aniScale, m_aniTranslate;
    float	m_left, m_right, m_top, m_bottom, m_mouseZ;

    //std::function<bool(Sprite*, Sprite*)> m_sorter;

    GraphicSystem(const GraphicSystem& /*_copy*/) = delete;
    void operator=(const GraphicSystem& /*_copy*/) = delete;
};

jeEnd
