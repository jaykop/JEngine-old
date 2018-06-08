#pragma once
#include "Vector4.h"
#include "Component.h"
#include "ComponentManager.h"
#include "ComponentBuilder.h"

// For enum ProjectType
#include "GraphicSystem.h"

jeBegin
jeDeclareComponentBuilder(Sprite);

class Transform;

class Sprite : public Component
{
    // Keyword Definitions
    jeBaseFriends(Sprite);
    friend class GraphicSystem;

    friend class Material;
    friend class Animation;

    friend class ModelBuilder;
    friend class SpriteBuilder;
    friend class AssetManager;

    using Image = std::vector<unsigned char>;
    using TextureMap = std::unordered_map<std::string, unsigned>;

protected:

	//const static int IS_MODEL = 0x00001;
	const static int IS_TEXT = 0x00010;
	const static int IS_EMITTER = 0x00100;
	const static int HAS_ANIMATION = 0x01000;
	const static int HAS_MATERIAL = 0x10000;

	int status;

public:

    void Register() override;

    void		AddTexture(const char* _key);
    void		RemoveTexture(const char* _key);
    void		SetCurrentTexutre(const char* _key);
    unsigned	GetCurrentTexutre();
    unsigned	GetTexutre(const char* _key);

    bool		flip, bilboard;
    vec4		color;
    ProjectType projection;

    unsigned	sfactor, dfactor;
    unsigned	*pVao, elementSize;

protected:

    ~Sprite();
    Sprite(Object* _pOwner);
    void operator=(const Sprite& _copy);

    void Load(CR_RJValue _data) override;

    bool		m_culled;
    unsigned	m_mainTex;
    TextureMap	m_textureMap;
    Transform	*m_pTransform;
    Material	*m_pMaterial;
    Animation	*m_pAnimation;

private:

    // Locked constuctors and destructor
    Sprite() = delete;
    Sprite(const Sprite& /*_copy*/) = delete;

    void EditorUpdate(const float _dt) override;
};

jeEnd
