#pragma once
#include "glew.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "GLManager.h"

jeBegin

class Shader {

private:

    enum Type { VERTEX, GEOMETRY, PIXEL };

    friend class GLManager;
    friend class GraphicSystem;

    static void Use(const GLManager::ShaderType& _shaderType);

    void CreateShader(std::string& _shaderContents, Type _type);
    void CombineShaders();

    // By const character
    void SetInt(const char * _name, int _int);
    void SetBool(const char* _name, bool _bool);
    void SetEnum(const char * _name, int _enum);
	void SetMatrix(const char * _name, const mat4 & _mat4);
    void SetuInt(const char*_name, unsigned _uInt);
    void SetFloat(const char * _name, float _float);
    void SetVector3(const char * _name, const vec3& _vector);
    void SetVector4(const char * _name, const vec4& _vector);

    Shader();
    ~Shader();

    static Shader* m_pCurrentShader;

    GLuint  m_programId, m_vertexId, m_fragmentId, m_geometryId;
    int	    m_infoLogLength;
    GLint   m_result;

    static std::string  m_vertexShader[GLM::SHADER_END], m_fragmentShader[GLM::SHADER_END], 
		m_geometryShader[GLM::SHADER_END];

    Shader(const Shader& /*_copy*/) = delete;
    void operator=(const Shader& /*_copy*/) = delete;

};

jeEnd
