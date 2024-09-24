#pragma once

#include <glad/glad.h>

#include <string>

namespace RenderEngine {

	class ShaderProgram
	{
	public:

		ShaderProgram(const std::string& vertexSource, const std::string& fragmentSource);
		~ShaderProgram();

		ShaderProgram(ShaderProgram&& shader) noexcept;
		ShaderProgram(ShaderProgram&) = delete;
		ShaderProgram(const ShaderProgram&&) = delete;

		ShaderProgram& operator=(ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;
		ShaderProgram& operator=(ShaderProgram&& shader) noexcept;

		bool isCompiled() const { return m_isCompiled; }
		void use() const;

		void setFloat(const float value, const std::string& name) const;
		void setInt(const int value, const std::string& name) const;

	private:

		bool createShader(GLuint& shaderID, const GLenum& shaderType, const std::string& shaderSource);

		bool m_isCompiled = false;
		GLuint m_ID;

	};

}