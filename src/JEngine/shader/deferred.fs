#version 450 core

struct LightInfo {
	vec4 Position;
	vec3 Intensity;
};

uniform LightInfo Light;

struct MaterialInfo {
	vec3 Kd;
};

uniform MaterialInfo Material;

subroutine void RenderPassType();
subroutine uniform RenderPassType RenderPass;

layout(binding=0) uniform sampler2D PositionTex;
layout(binding=1) uniform sampler2D ColorTex;
layout(binding=2) uniform sampler2D NormalTex;

in vec3 v3_outPosition;
in vec2 v2_outTexCoord;
in vec3 v3_outNormal;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec3 PositionData;
layout (location = 2) out vec3 ColorData;
layout (location = 3) out vec3 NormalData;

vec3 DiffuseModel (vec3 _position, vec3 _normal, vec3 _diff)
{
	vec3 s = normalize(vec3(Light.Position) - _position);
	float sDotN = max(dot(s, _normal), 0.0);
	vec3 diffuse = Light.Intensity * _diff * sDotN;

	return diffuse;
}

subroutine (RenderPassType)
void render1()
{
	// Store position, normal, and diffuse color in g-buffer
	PositionData = 2 * v3_outPosition;
	ColorData = Material.Kd;
	NormalData = v3_outNormal;
}

subroutine (RenderPassType)
void render2()
{
	// Retrieve position, normal and color information from
	// the g-buffer textures
	vec2 newTex = v2_outTexCoord;
	newTex.y = 1 - newTex.y;
	vec3 pos = vec3(texture(PositionTex, newTex));
	vec3 diffColor = vec3(texture(ColorTex, newTex));
	vec3 norm = vec3(texture(NormalTex, newTex));
	
	FragColor = vec4(DiffuseModel(pos, norm, diffColor), 1.0);
	
}

void main()
{
	RenderPass();
}