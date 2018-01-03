#pragma once
#include "Macro.h"
#include "glew.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "GLManager.h"

JE_BEGIN

class Shader {

private:

	friend class GLManager;
	friend class GraphicSystem;

	void LoadShader(
		const char* _vertex_file_path,
		const char* _fragment_file_path);
	
	void Use();

	// By enum
	void SetFloat(GLManager::UniformType _uniform, float _float);
	void ConnectUniform(GLManager::UniformType _uniform, const char* _name);
	void SetMatrix(GLManager::UniformType _uniform, const mat4& _matrix);
	void SetVector4(GLManager::UniformType _uniform, const vec4& _vector);
	void SetVector3(GLManager::UniformType _uniform, const vec3& _vector);
	void SetVector2(GLManager::UniformType _uniform, float x, float y);
	void SetBool(GLManager::UniformType _uniform, bool _bool);
	void SetEnum(GLManager::UniformType _uniform, int _int);
	void SetInt(GLManager::UniformType _uniform, int _int);

	// By const character
	void SetVector4(const char * _name, const vec4 & _vector);
	void SetVector3(const char * _name, const vec3 & _vector);
	void SetInt(const char * _name, int _int);
	void SetFloat(const char * _name, float _float);
	void SetEnum(const char * _name, int _int);

	Shader() :m_programId(0) {};
	~Shader() {};

	GLuint m_programId;

	Shader(const Shader& /*_copy*/) = delete;
	void operator=(const Shader& /*_copy*/) = delete;

};

JE_END
