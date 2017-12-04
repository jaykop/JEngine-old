#pragma once
#include "Macro.h"
#include "GL\glew.h"
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

	void SetFloat(GLManager::UniformType _uniform, float _float);
	void ConnectUniform(GLManager::UniformType _uniform, const char* _name);
	void SetMatrix(GLManager::UniformType _uniform, const mat4& _matrix);
	void SetVector4(GLManager::UniformType _uniform, const vec4& _vector);
	void SetVector3(GLManager::UniformType _uniform, const vec3& _vector);
	void SetBool(GLManager::UniformType _uniform, bool _bool);
	void SetEnum(GLManager::UniformType _uniform, int _int);
	void SetInt(GLManager::UniformType _uniform, int _int);

	Shader() :m_programId(0) {};
	~Shader() {};

	GLuint m_programId;

	Shader(const Shader& /*_copy*/) = delete;
	void operator=(const Shader& /*_copy*/) = delete;

	//void SetFloat(GLint& _uniform, float _float);
	//void ConnectUniform(GLint& _uniform, const char* _name);
	//void SetMatrix(GLint& _uniform, const mat4& _matrix);
	//void SetVector4(GLint& _uniform, const vec4& _vector);
	//void SetVector3(GLint& _uniform, const vec3& _vector);
	//void SetBool(GLint& _uniform, bool _bool);
	//void SetEnum(GLint& _uniform, int _int);
	//void SetInt(GLint& _uniform, int _int);
};

JE_END
