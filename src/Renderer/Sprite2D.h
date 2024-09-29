#pragma once

#include <string>
#include <memory>

#include <glad/glad.h>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "VertexBuffer.h"

namespace RenderEngine
{
	class ShaderProgram;
	class Texture2D;

	class Sprite2D
	{
	public:

		Sprite2D(std::shared_ptr<ShaderProgram> pShaderProgram, std::shared_ptr<Texture2D> pTexture, std::string initialSubTexture="default");

		~Sprite2D();

		Sprite2D(const Sprite2D&) = delete;
		Sprite2D& operator=(const Sprite2D&) = delete;
		Sprite2D& operator=(Sprite2D&&) = delete;
		Sprite2D(Sprite2D&&) = delete;

		void render(const glm::vec2& position, const glm::vec2& scale, const int layer, const float rotation) const;

	private:

		GLuint m_VAO, m_EBO;

		VertexBuffer m_vertexBuffer;

		std::shared_ptr<Texture2D> m_pTexture;
		std::shared_ptr<ShaderProgram> m_pShaderProgram;

	};
}