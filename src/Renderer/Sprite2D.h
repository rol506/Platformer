#pragma once

#include <string>
#include <memory>

#include <glad/glad.h>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

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

		void setTargetPosition(const glm::vec2& targetPosition);
		void setPosition(const glm::vec2& position);
		void addVelocity(const glm::vec2& velocity);
		void setSize(const glm::vec2& size);
		glm::vec2 getPosition() const { return m_position; }
		glm::vec2 getTargetPosition() const { return m_targetPosition; }
		glm::vec2 getVelocity() const { return m_velocity; }
		glm::vec2 getSize() const { return m_size; }

	private:

		VertexArray m_vertexArray;
		VertexBuffer m_vertexBuffer;
		IndexBuffer m_indexBuffer;

		glm::vec2 m_position;
		glm::vec2 m_targetPosition;
		glm::vec2 m_velocity;
		glm::vec2 m_size;

		std::shared_ptr<Texture2D> m_pTexture;
		std::shared_ptr<ShaderProgram> m_pShaderProgram;

	};
}