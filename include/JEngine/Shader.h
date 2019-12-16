#pragma once
#include "glew.h"
#include "Vector3.h"
#include "Matrix4x4.h"
#include "GLManager.h"

jeBegin

class Shader {

private:

    enum Type { JE_VERTEX, JE_GEOMETRY, JE_PIXEL };

    friend class GLManager;
    friend class GraphicSystem;
	friend class AssetManager;

    static void Use(const GLManager::ShaderType& shaderType);

    void CreateShader(std::string& shaderContents, Type type);
    void CombineShaders();

    // By const character
    void SetInt(const char * name, int number);
    void SetBool(const char* name, bool toggle);
    void SetEnum(const char * name, int typr);
	void SetMatrix(const char * name, const mat4& mat4);
    void SetuInt(const char*name, unsigned uInt);
    void SetFloat(const char * name, float number);
    void SetVector3(const char * name, const vec3& vector);
    void SetVector4(const char * name, const vec4& vector);

    Shader();
    ~Shader() {};

    static Shader* pCurrentShader_;

    GLuint  programId_, vertexId_, fragmentId_, geometryId_;
    int	    infoLogLength_;
    GLint   result_;

    static std::string  vertexShader_[GLM::JE_SHADER_END], fragmentShader_[GLM::JE_SHADER_END], 
		m_geometryShader[GLM::JE_SHADER_END];

    Shader(const Shader& /*copy*/) = delete;
    void operator=(const Shader& /*copy*/) = delete;

};

jeEnd
