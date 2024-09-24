#include "ShaderProgram.h"

#include <iostream>

namespace RenderEngine 
{
	bool ShaderProgram::createShader(GLuint& shaderID, const GLenum& shaderType, const std::string& shaderSource)
	{
		shaderID = glCreateShader(shaderType);
		const char* code = shaderSource.c_str();
		glShaderSource(shaderID, 1, &code, nullptr);
		glCompileShader(shaderID);

		GLint success; 
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			GLchar infoLog[1024];
			glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);
			std::clog << "Shader compilation failed: " << infoLog << "\n";
			return false;
		}

		return true;
	}

	ShaderProgram::ShaderProgram(const std::string& vertexSource, const std::string& fragmentSource)
	{
		GLuint vertexID;
		if (!createShader(vertexID, GL_VERTEX_SHADER, vertexSource))
		{
			std::cerr << "Failed to create vertex shader!\n";
			return;
		}

		GLuint fragmentID;
		if (!createShader(fragmentID, GL_FRAGMENT_SHADER, fragmentSource))
		{
			std::cerr << "Failed to create fragment shader!\n";
			glDeleteShader(vertexID);
			return;
		}

		m_ID = glCreateProgram();
		glAttachShader(m_ID, vertexID);
		glAttachShader(m_ID, fragmentID);
		glLinkProgram(m_ID);

		GLint success;
		glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			GLchar infoLog[1024];
			glGetProgramInfoLog(m_ID, 1024, nullptr, infoLog);
			std::clog << "Program linking failed: " << infoLog << "\n";
		}
		else m_isCompiled = true;

		glDeleteShader(vertexID);
		glDeleteShader(fragmentID);
	}

	void ShaderProgram::use() const
	{
		glUseProgram(m_ID);
	}

	void ShaderProgram::setFloat(const float value, const std::string& name) const
	{
		glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
	}

	void ShaderProgram::setInt(const int value, const std::string& name) const
	{
		glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
	}

	ShaderProgram::ShaderProgram(ShaderProgram&& shader) noexcept
	{
		m_ID = shader.m_ID;
		m_isCompiled = shader.m_isCompiled;

		shader.m_ID = 0;
		shader.m_isCompiled = false;
	}

	ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shader) noexcept
	{
		if (this != &shader)
		{
			glDeleteProgram(m_ID);

			m_ID = shader.m_ID;
			m_isCompiled = shader.m_isCompiled;

			shader.m_ID = 0;
			shader.m_isCompiled = false;
		}
		return *this;
	}

	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(m_ID);
	}
}