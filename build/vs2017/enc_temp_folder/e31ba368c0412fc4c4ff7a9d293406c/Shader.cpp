#include <vector>
#include <string>
#include "Shader.h"

jeBegin

Shader* Shader::m_pCurrentShader = nullptr;

void Shader::Use(const GLM::ShaderType& _shaderType)
{
	m_pCurrentShader = GLM::m_shader[_shaderType];
	glUseProgram(m_pCurrentShader->m_programId);
}

Shader::Shader()
	:m_programId(0), m_vertexId(0),
	m_fragmentId(0), m_geometryId(0),
	m_infoLogLength(0), m_result(GL_FALSE)
{}

Shader::~Shader() {}

void Shader::CreateShader(std::string& _shaderContents, Type _type)
{
	// Create the shader
	GLuint *pShader = nullptr;

	switch (_type)
	{
	case VERTEX:
		m_vertexId = glCreateShader(GL_VERTEX_SHADER);
		pShader = &m_vertexId;
		break;
	case PIXEL:
		m_fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
		pShader = &m_fragmentId;
		break;
	case GEOMETRY:
		m_geometryId = glCreateShader(GL_GEOMETRY_SHADER);
		pShader = &m_geometryId;
		break;
	}

	char const* SourcePointer = _shaderContents.c_str();
	glShaderSource(*pShader, 1, &SourcePointer, nullptr);
	glCompileShader(*pShader);

	// Check shader
	glGetShaderiv(*pShader, GL_COMPILE_STATUS, &m_result);
	glGetShaderiv(*pShader, GL_INFO_LOG_LENGTH, &m_infoLogLength);

	if (m_infoLogLength > 0) {
		std::vector<char> ShaderErrorMessage(m_infoLogLength + 1);
		glGetShaderInfoLog(*pShader, m_infoLogLength, nullptr, &ShaderErrorMessage[0]);
		jeDebugPrint("!Shader - %4s\n", &ShaderErrorMessage[0]);
	}
}

void Shader::CombineShaders()
{
	m_programId = glCreateProgram();

	if (m_programId == 0)
		jeDebugPrint("!Shader - Shader couldn't get program id.\n");

	else {

		// Combine several shaders into the program
		if (m_vertexId)
			glAttachShader(m_programId, m_vertexId);
		if (m_fragmentId)
			glAttachShader(m_programId, m_fragmentId);
		if (m_geometryId)
			glAttachShader(m_programId, m_geometryId);

		glLinkProgram(m_programId);

		// Check the program
		glGetProgramiv(m_programId, GL_LINK_STATUS, &m_result);
		glGetShaderiv(m_programId, GL_INFO_LOG_LENGTH, &m_infoLogLength);

		// Check if linked properly
		if (m_infoLogLength > 0) {
			std::vector<char> ProgramErrorMessage(m_infoLogLength + 1);
			glGetShaderInfoLog(m_programId, m_infoLogLength, nullptr, &ProgramErrorMessage[0]);
			jeDebugPrint("!Shader: %4s\n", &ProgramErrorMessage[0]);

		}	// if (infoLogLength > 0) {

		glUseProgram(m_programId);	// Start using shade  program

		if (m_vertexId) {
			glDetachShader(m_programId, m_vertexId);
			glDeleteShader(m_vertexId);
		}

		if (m_fragmentId) {
			glDetachShader(m_programId, m_fragmentId);
			glDeleteShader(m_fragmentId);
		}

		if (m_geometryId) {
			glDetachShader(m_programId, m_geometryId);
			glDeleteShader(m_geometryId);
		}
	}
}
//void Shader::SetFloat(GLManager::UniformType _uniform, float _float)
//{
//	glUniform1f(GLManager::m_uniform[_uniform], _float);
//}
//
//void Shader::SetuInt(GLManager::UniformType _uniform, unsigned _uInt)
//{
//	glUniform1ui(GLManager::m_uniform[_uniform], _uInt);
//}
//
//void Shader::ConnectUniform(GLManager::UniformType _uniform, const char * _name)
//{
//	GLManager::m_uniform[_uniform]
//		= glGetUniformLocation(m_programId, _name);
//}
//
//void Shader::SetMatrix(GLManager::UniformType _uniform, const mat4& _matrix)
//{
//	glUniformMatrix4fv(GLManager::m_uniform[_uniform],
//		1, GL_FALSE, &_matrix.m[0][0]);
//}
//
//void Shader::SetVector4(GLManager::UniformType _uniform, const vec4& _vector)
//{
//	glUniform4f(GLManager::m_uniform[_uniform],
//		_vector.x, _vector.y, _vector.z, _vector.w);
//}
//
//void Shader::SetVector3(GLManager::UniformType _uniform, const vec3& _vector)
//{
//	glUniform3f(GLManager::m_uniform[_uniform],
//		_vector.x, _vector.y, _vector.z);
//}
//
//void Shader::SetVector2(GLManager::UniformType _uniform, float x, float y)
//{
//	glUniform2f(GLManager::m_uniform[_uniform], x, y);
//}
//
//void Shader::SetBool(GLManager::UniformType _uniform, bool _bool)
//{
//	glUniform1i(GLManager::m_uniform[_uniform], _bool);
//}
//
//void Shader::SetEnum(GLManager::UniformType _uniform, int _enum)
//{
//	glUniform1i(GLManager::m_uniform[_uniform], _enum);
//}
//
//void Shader::SetInt(GLManager::UniformType _uniform, int _int)
//{
//	glUniform1i(GLManager::m_uniform[_uniform], _int);
//}

void Shader::SetBool(const char* _name, bool _bool)
{
	glUniform1i(glGetUniformLocation(m_pCurrentShader->m_programId, _name), _bool);
}

void Shader::SetuInt(const char * _name, unsigned _uInt)
{
	glUniform1ui(glGetUniformLocation(m_pCurrentShader->m_programId, _name), _uInt);
}

void Shader::SetVector4(const char* _name, const vec4& _vector)
{
	glUniform4f(glGetUniformLocation(m_pCurrentShader->m_programId, _name),
		_vector.x, _vector.y, _vector.z, _vector.w);
}

void Shader::SetInt(GLint& _buffer, int _int)
{
	glUniform1i(_buffer, _int);
}

void Shader::SetEnum(GLint& _buffer, int _enum)
{
	glUniform1i(_buffer, _enum);
}

void Shader::SetBool(GLint& _buffer, bool _bool)
{
	glUniform1i(_buffer, _bool);
}

void Shader::SetFloat(GLint& _buffer, float _float)
{
	glUniform1f(_buffer, _float);
}

void Shader::SetuInt(GLint& _buffer, unsigned _uInt)
{
	glUniform1ui(_buffer, _uInt);
}

void Shader::SetVector2(GLint& _buffer, float _x, float _y)
{
	glUniform2f(_buffer, _x, _y);
}

void Shader::SetVector3(GLint& _buffer, const vec3 & _vector)
{
	glUniform3f(_buffer,
		_vector.x, _vector.y, _vector.z);
}

void Shader::SetVector4(GLint& _buffer, const vec4 & _vector)
{
	glUniform4f(_buffer,
		_vector.x, _vector.y, _vector.z, _vector.w);
}

void Shader::SetVector3(const char* _name, const vec3& _vector)
{
	glUniform3f(glGetUniformLocation(m_pCurrentShader->m_programId, _name),
		_vector.x, _vector.y, _vector.z);
}

void Shader::SetInt(const char* _name, int _int)
{
	glUniform1i(glGetUniformLocation(m_pCurrentShader->m_programId, _name), _int);
}

void Shader::SetFloat(const char* _name, float _float)
{
	glUniform1f(glGetUniformLocation(m_pCurrentShader->m_programId, _name), _float);
}

void Shader::SetEnum(const char* _name, int _enum)
{
	glUniform1i(glGetUniformLocation(m_pCurrentShader->m_programId, _name), _enum);
}

void Shader::SetMatrix(const char* _name, const mat4& _mat4)
{
	glUniformMatrix4fv(glGetUniformLocation(m_pCurrentShader->m_programId, _name), 1, GL_FALSE, &_mat4.m[0][0]);
}

jeEnd
