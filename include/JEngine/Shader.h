#pragma once
#include "glew.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "GLManager.h"

JE_BEGIN

class Shader {

private:

	friend class GLManager;
	friend class GraphicSystem;

	//void LoadShader(
	//	const char* _vertex_file_path,
	//	const char* _fragment_file_path);

	void EnterShader(
		std::string& _vertexShader, std::string& _fragmentShader
		/*, const char* _vertexPath = nullptr, const char* _fragmentPath = nullptr*/
	);

	void Use();
	void ConnectUniform(GLManager::UniformType _uniform, const char* _name);

	// By enum	
	void SetInt(GLManager::UniformType _uniform, int _int);
	void SetEnum(GLManager::UniformType _uniform, int _enum);
	void SetBool(GLManager::UniformType _uniform, bool _bool);
	void SetFloat(GLManager::UniformType _uniform, float _float);
	void SetuInt(GLManager::UniformType _uniform, unsigned _uInt);
	void SetVector2(GLManager::UniformType _uniform, float x, float y);
	void SetMatrix(GLManager::UniformType _uniform, const mat4& _matrix);
	void SetVector3(GLManager::UniformType _uniform, const vec3& _vector);
	void SetVector4(GLManager::UniformType _uniform, const vec4& _vector);
	
	// By const character
	void SetInt(const char * _name, int _int);
	void SetBool(const char* _name, bool _bool);
	void SetEnum(const char * _name, int _enum);
	void SetuInt(const char*_name, unsigned _uInt);
	void SetFloat(const char * _name, float _float);
	void SetVector3(const char * _name, const vec3& _vector);
	void SetVector4(const char * _name, const vec4& _vector);

	// By buffer
	void SetInt(GLint& _buffer, int _int);
	void SetEnum(GLint& _buffer, int _enum);
	void SetBool(GLint& _buffer, bool _bool);
	void SetFloat(GLint& _buffer, float _float);
	void SetuInt(GLint& _buffer, unsigned _uInt);
	void SetVector2(GLint& _buffer, float _x, float _y);
	void SetVector3(GLint& _buffer, const vec3& _vector);
	void SetVector4(GLint& _buffer, const vec4& _vector);
	
	Shader() :m_programId(0) {};
	~Shader() {};

	GLuint m_programId;

	static std::string  m_shaderVertex[GLM::SHADER_END], m_shaderFragment[GLM::SHADER_END];

	Shader(const Shader& /*_copy*/) = delete;
	void operator=(const Shader& /*_copy*/) = delete;

};

JE_END
