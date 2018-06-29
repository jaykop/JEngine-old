#pragma once
#include "Component.h"
#include "ComponentManager.h"
#include "ComponentBuilder.h"

// For enum ProjectType
#include "GraphicSystem.h"

jeBegin
jeDeclareComponentBuilder(Model);

class Mesh;
class Transform;

class Model : public Component
{
    // Keyword Definitions
    jeBaseFriends(Model);
    friend class GraphicSystem;
    friend class Material;
    friend class Animation;
    friend class AssetManager;

    using TextureMap = std::unordered_map<std::string, unsigned>;
	unsigned	m_mainTex;
	TextureMap	m_textureMap;

protected:

    const static int IS_TEXT = 0x10;
    const static int IS_EMITTER = 0x01;
	const static int IS_LIGHT = 0x11;

	std::vector<Mesh*>	meshes_;
    int					is_;

public:

    const static int IS_FLIPPED = 0x100;
    const static int IS_BILBOARD = 0x010;
    const static int IS_INHERITED = 0x001;

    void Register() override;

	void		AddMesh(Mesh* pMesh);
	void		RemoveMesh(unsigned index);
	Mesh*		GetMesh(unsigned index) const;
	unsigned	GetMeshCount() const;

    void		SetParentToFollow(Object* _pObj);
    void		AddTexture(const char* _key);
    void		RemoveTexture(const char* _key);
    void		SetCurrentTexutre(const char* _key);
    unsigned	GetCurrentTexutre() const;
    unsigned	GetTexutre(const char* _key);

	unsigned	m_drawMode;
    int			status;
    vec4		color;
    ProjectType projection;
    unsigned	sfactor, dfactor;

protected:

    ~Model();
	Model(Object* _pOwner);
    void operator=(const Model& _copy);

    void Load(CR_RJValue _data) override;

    bool		m_culled;
    Transform	*m_pTransform, *m_pInherited;
    Material	*m_pMaterial;
    Animation	*m_pAnimation;

private:

    // Locked constuctors and destructor
	Model() = delete;
	Model(const Model& /*_copy*/) = delete;

    void EditorUpdate(float dt) override;
};

jeEnd
