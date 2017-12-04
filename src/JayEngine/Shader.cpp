#include <vector>
#include <string>
#include <fstream>
#include "Shader.hpp"

JE_BEGIN

void Shader::LoadShader(
	const char* _vertex_file_path, 
	const char* _fragment_file_path) {

	// Both strings are should be valid
	if (_vertex_file_path && _fragment_file_path) {

		bool vsToggle = true, fsToggle = true;

		// Create the shader
		GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

		// Read the vertex shader code from the file
		std::string vertexShaderCode;
		std::ifstream vertexShaderStream(_vertex_file_path, std::ios::in);

		// Read the fragment shader code from the file
		std::string fragmentShaderCode;
		std::ifstream fragmentShaderStream(_fragment_file_path, std::ios::in);

		GLint result = GL_FALSE;
		int infoLogLength;

		// If the vertex shader file is valid,
		if (vertexShaderStream.is_open()) {
			std::string line = "";
			while (std::getline(vertexShaderStream, line))
				vertexShaderCode += "\n" + line;
			vertexShaderStream.close();

		}	// if (vertexShaderStream.is_open()) {

		else {
			JE_DEBUG_PRINT("Impossible to open %s\n", _vertex_file_path);
			vsToggle = false;

		}	// else {

		// If vertex file is valid
		// load fragement shader
		if (vsToggle) {

			// If the fragment shader file is valid,
			if (fragmentShaderStream.is_open()) {
				std::string line = "";
				while (std::getline(fragmentShaderStream, line))
					fragmentShaderCode += "\n" + line;
				fragmentShaderStream.close();

			}	// if (fragmentShaderStream.is_open()) {

			else {
				JE_DEBUG_PRINT("Impossible to open %s\n", _fragment_file_path);
				fsToggle = false;

			}	// else {

		}	// if (vsToggle) {

		// So both shaders are valid and compatible,
		if (vsToggle && fsToggle) {

			// Complie vertex shader
			JE_DEBUG_PRINT("Compiling shader: %s\n", _vertex_file_path);
			char const* vertexSourcePointer = vertexShaderCode.c_str();
			glShaderSource(vertexShaderId, 1, &vertexSourcePointer, NULL);
			glCompileShader(vertexShaderId);

			// Check vertex shader
			glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &result);
			glGetShaderiv(vertexShaderId, GL_INFO_LOG_LENGTH, &infoLogLength);

			if (infoLogLength > 0) {
				std::vector<char> VertexShaderErrorMessage(infoLogLength + 1);
				glGetShaderInfoLog(vertexShaderId, infoLogLength, NULL, &VertexShaderErrorMessage[0]);
				JE_DEBUG_PRINT("%4s\n", &VertexShaderErrorMessage[0]);

			}	// if (infoLogLength > 0) {

			// Complie fragment shader
			JE_DEBUG_PRINT("Compiling shader: %s\n", _fragment_file_path);

			// Get shader string
			char const* fragmentSourcePointer = fragmentShaderCode.c_str();
			glShaderSource(fragmentShaderId, 1, &fragmentSourcePointer, NULL);
			glCompileShader(fragmentShaderId);

			// Check fragment shader
			glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &result);
			glGetShaderiv(fragmentShaderId, GL_INFO_LOG_LENGTH, &infoLogLength);

			// Check if copiled properly
			if (infoLogLength > 0) {
				std::vector<char> FragShaderErrorMessage(infoLogLength + 1);
				glGetShaderInfoLog(fragmentShaderId, infoLogLength, NULL, &FragShaderErrorMessage[0]);
				JE_DEBUG_PRINT("%4s\n", &FragShaderErrorMessage[0]);

			}	// if (infoLogLength > 0) {

			// Link the program
			JE_DEBUG_PRINT("Linking program...\n");
			m_programId = glCreateProgram();

			// Combine two shaders into the program
			glAttachShader(m_programId, vertexShaderId);
			glAttachShader(m_programId, fragmentShaderId);
			glLinkProgram(m_programId);

			// Check the program
			glGetProgramiv(m_programId, GL_LINK_STATUS, &result);
			glGetShaderiv(m_programId, GL_INFO_LOG_LENGTH, &infoLogLength);

			// Check if linked properly
			if (infoLogLength > 0) {
				std::vector<char> ProgramErrorMessage(infoLogLength + 1);
				glGetShaderInfoLog(m_programId, infoLogLength, NULL, &ProgramErrorMessage[0]);
				JE_DEBUG_PRINT("%4s\n", &ProgramErrorMessage[0]);

			}	// if (infoLogLength > 0) {

			glUseProgram(m_programId);	// Start using shade  program

			glDetachShader(m_programId, vertexShaderId);
			glDetachShader(m_programId, fragmentShaderId);

			glDeleteShader(vertexShaderId);
			glDeleteShader(fragmentShaderId);

		}	// if (vsToggle && fsToggle) {

	}	// if (_vertex_file_path && _fragment_file_path) {

	else 
		JE_DEBUG_PRINT("Shader directory is not valid...\n");
}

void Shader::Use()
{
	glUseProgram(m_programId);
}

void Shader::SetFloat(GLManager::UniformType _uniform, float _float)
{
	glUniform1f(GLManager::m_uniform[_uniform], _float);
}

void Shader::ConnectUniform(GLManager::UniformType _uniform, const char * _name)
{
	GLManager::m_uniform[_uniform]
		= glGetUniformLocation(m_programId, _name);
}

void Shader::SetMatrix(GLManager::UniformType _uniform, const mat4& _matrix)
{
	glUniformMatrix4fv(GLManager::m_uniform[_uniform], 
		1, GL_FALSE, &_matrix.m_member[0][0]);
}

void Shader::SetVector4(GLManager::UniformType _uniform, const vec4& _vector)
{
	glUniform4f(GLManager::m_uniform[_uniform], 
		_vector.x, _vector.y, _vector.z, _vector.w);
}

void Shader::SetVector3(GLManager::UniformType _uniform, const vec3 & _vector)
{
	glUniform3f(GLManager::m_uniform[_uniform], 
		_vector.x, _vector.y, _vector.z);
}

void Shader::SetBool(GLManager::UniformType _uniform, bool _bool)
{
	glUniform1i(GLManager::m_uniform[_uniform], _bool);
}

void Shader::SetEnum(GLManager::UniformType _uniform, int _int)
{
	glUniform1i(GLManager::m_uniform[_uniform], _int);
}

void Shader::SetInt(GLManager::UniformType _uniform, int _int)
{
	glUniform1d(GLManager::m_uniform[_uniform], _int);
}

//void Shader::SetFloat(GLint& _uniform, float _float)
//{
//	glUniform1f(_uniform, _float);
//}
//
//void Shader::ConnectUniform(GLint& _uniform, const char * _name)
//{
//	_uniform = glGetUniformLocation(m_programId, _name);
//}
//
//void Shader::SetMatrix(GLint& _uniform, const mat4& _matrix)
//{
//	glUniformMatrix4fv(_uniform, 1, GL_FALSE, &_matrix.m_member[0][0]);
//}
//
//void Shader::SetVector4(GLint& _uniform, const vec4& _vector)
//{
//	glUniform4f(_uniform, _vector.x, _vector.y, _vector.z, _vector.w);
//}
//
//void Shader::SetVector3(GLint& _uniform, const vec3 & _vector)
//{
//	glUniform3f(_uniform, _vector.x, _vector.y, _vector.z);
//}
//
//void Shader::SetBool(GLint& _uniform, bool _bool)
//{
//	glUniform1i(_uniform, _bool);
//}
//
//void Shader::SetEnum(GLint& _uniform, int _int)
//{
//	glUniform1i(_uniform, _int);
//}
//
//void Shader::SetInt(GLint& _uniform, int _int)
//{
//	glUniform1d(_uniform, _int);
//}

JE_END
