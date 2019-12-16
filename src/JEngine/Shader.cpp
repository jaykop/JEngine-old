#include <vector>
#include <string>
#include "Shader.h"

jeBegin

Shader* Shader::pCurrentShader_ = nullptr;

void Shader::Use(const GLM::ShaderType& shaderType)
{
	pCurrentShader_ = GLM::shader_[shaderType];
	glUseProgram(pCurrentShader_->programId_);
}

Shader::Shader()
	:programId_(0), vertexId_(0),
	fragmentId_(0), geometryId_(0),
	infoLogLength_(0), result_(GL_FALSE)
{}

void Shader::CreateShader(std::string& shaderContents, Type type)
{
	// Create the shader
	GLuint *pShader = nullptr;

	switch (type)
	{
	case JE_VERTEX:
		vertexId_ = glCreateShader(GL_VERTEX_SHADER);
		pShader = &vertexId_;
		break;
	case JE_PIXEL:
		fragmentId_ = glCreateShader(GL_FRAGMENT_SHADER);
		pShader = &fragmentId_;
		break;
	case JE_GEOMETRY:
		geometryId_ = glCreateShader(GL_GEOMETRY_SHADER);
		pShader = &geometryId_;
		break;
	}

	char const* SourcePointer = shaderContents.c_str();
	glShaderSource(*pShader, 1, &SourcePointer, nullptr);
	glCompileShader(*pShader);

	// Check shader
	glGetShaderiv(*pShader, GL_COMPILE_STATUS, &result_);
	glGetShaderiv(*pShader, GL_INFO_LOG_LENGTH, &infoLogLength_);

	if (infoLogLength_ > 0) {
		std::vector<char> ShaderErrorMessage(infoLogLength_ + 1);
		glGetShaderInfoLog(*pShader, infoLogLength_, nullptr, &ShaderErrorMessage[0]);
		jeDebugPrint("!Shader - %4s\n", &ShaderErrorMessage[0]);
	}
}

void Shader::CombineShaders()
{
	programId_ = glCreateProgram();

	if (programId_ == 0)
		jeDebugPrint("!Shader - Shader couldn't get program id.\n");

	else {

		// Combine several shaders into the program
		if (vertexId_)
			glAttachShader(programId_, vertexId_);
		if (fragmentId_)
			glAttachShader(programId_, fragmentId_);
		if (geometryId_)
			glAttachShader(programId_, geometryId_);

		glLinkProgram(programId_);

		// Check the program
		glGetProgramiv(programId_, GL_LINK_STATUS, &result_);
		glGetShaderiv(programId_, GL_INFO_LOG_LENGTH, &infoLogLength_);

		// Check if linked properly
		if (infoLogLength_ > 0) {
			std::vector<char> ProgramErrorMessage(infoLogLength_ + 1);
			glGetShaderInfoLog(programId_, infoLogLength_, nullptr, &ProgramErrorMessage[0]);
			jeDebugPrint("!Shader: %4s\n", &ProgramErrorMessage[0]);

		}	// if (infoLogLength > 0) {

		glUseProgram(programId_);	// Start using shade  program

		if (vertexId_) {
			glDetachShader(programId_, vertexId_);
			glDeleteShader(vertexId_);
		}

		if (fragmentId_) {
			glDetachShader(programId_, fragmentId_);
			glDeleteShader(fragmentId_);
		}

		if (geometryId_) {
			glDetachShader(programId_, geometryId_);
			glDeleteShader(geometryId_);
		}
	}
}

void Shader::SetBool(const char* name, bool toggle)
{
	glUniform1i(glGetUniformLocation(pCurrentShader_->programId_, name), toggle);
}

void Shader::SetuInt(const char * name, unsigned uInt)
{
	glUniform1ui(glGetUniformLocation(pCurrentShader_->programId_, name), uInt);
}

void Shader::SetVector4(const char* name, const vec4& vector)
{
	glUniform4f(glGetUniformLocation(pCurrentShader_->programId_, name),
		vector.x, vector.y, vector.z, vector.w);
}

void Shader::SetVector3(const char* name, const vec3& vector)
{
	glUniform3f(glGetUniformLocation(pCurrentShader_->programId_, name),
		vector.x, vector.y, vector.z);
}

void Shader::SetInt(const char* name, int number)
{
	glUniform1i(glGetUniformLocation(pCurrentShader_->programId_, name), number);
}

void Shader::SetFloat(const char* name, float number)
{
	glUniform1f(glGetUniformLocation(pCurrentShader_->programId_, name), number);
}

void Shader::SetEnum(const char* name, int type)
{
	glUniform1i(glGetUniformLocation(pCurrentShader_->programId_, name), type);
}

void Shader::SetMatrix(const char* name, const mat4& mat4)
{
	glUniformMatrix4fv(glGetUniformLocation(pCurrentShader_->programId_, name), 
		1, GL_FALSE, &mat4.m[0][0]);
}

jeEnd
