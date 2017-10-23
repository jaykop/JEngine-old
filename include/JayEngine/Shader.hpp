#pragma once
#include "Macro.h"
#include "GL\glew.h"
#include "Vector4.h"
#include "Matrix4x4.h"

JE_BEGIN

class Shader {

private:

	friend class GLManager;
	friend class GraphicSystem;

	void LoadShader(
		const char* _vertex_file_path,
		const char* _fragment_file_path);
	
	void Use();
	void ConnectUniform(GLint& _uniform, const char* _name);
	void SetMatrix(GLint& _uniform, const mat4& _matrix);
	void SetVector4(GLint& _uniform, const vec4& _vector);
	void SetVector3(GLint& _uniform, const vec3& _vector);
	void SetBool(GLint& _uniform, bool _bool);

	Shader() {};
	~Shader() {};
	Shader(const Shader& /*_copy*/) {};
	void operator=(const Shader& /*_copy*/) {};

	GLuint m_programId;
};

JE_END
