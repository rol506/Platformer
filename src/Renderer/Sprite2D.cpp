#include "Sprite2D.h"

#include "Texture2D.h"
#include "ShaderProgram.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace RenderEngine
{
	Sprite2D::Sprite2D(std::shared_ptr<ShaderProgram> pShaderProgram, std::shared_ptr<Texture2D> pTexture, std::string initialSubTexture)
		: m_pShaderProgram(pShaderProgram), m_pTexture(pTexture)
	{
		auto subTexture = m_pTexture->getSubTexture(initialSubTexture);

		const float vertecies[] =
		{
			// 0 - 3
			// | / |
			// 1 - 2

			//X Y U V
			0.0f, 0.0f, subTexture.leftBottomUV.x, subTexture.rightTopUV.y,   // 0
			0.0f, 1.0f, subTexture.leftBottomUV.x, subTexture.leftBottomUV.y, // 1
			1.0f, 1.0f, subTexture.rightTopUV.x,   subTexture.leftBottomUV.y, // 2
			1.0f, 0.0f, subTexture.rightTopUV.x,   subTexture.rightTopUV.y    // 3
		};

		//const float vertecies[] =
		//{
		//	// 0 - 3
		//	// | \ |
		//	// 1 - 2

		//	//X Y U V
		//	0.0f, 0.0f, 0.0f, 1.0f,  // 0
		//	0.0f, 1.0f, 0.0f, 0.0f,  // 1
		//	1.0f, 1.0f, 1.0f, 0.0f,  // 2
		//	1.0f, 0.0f, 1.0f, 1.0f   // 3
		//};

		const unsigned int indices[] =
		{
			0, 1, 3,
			1, 3, 2
		};

		m_vertexBuffer.init(vertecies, sizeof(vertecies));

		glGenBuffers(1, &m_EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);
		m_vertexBuffer.bind();
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);

		m_vertexBuffer.unbind();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	Sprite2D::~Sprite2D()
	{
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_EBO);
	}

	void Sprite2D::render(const glm::vec2& position, const glm::vec2& scale, const int layer, const float rotation) const
	{
		m_pShaderProgram->use();

		glm::mat4 model(1.f);

		model = glm::translate(model, glm::vec3(position, 0.0f));
		model = glm::translate(model, glm::vec3(0.5f * scale.x, 0.5f * scale.y, 0.0f));
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-0.5f * scale.x, -0.5f * scale.y, 0.0f));
		model = glm::scale(model, glm::vec3(scale, 1.0f));
		model = glm::scale(model, glm::vec3(100, 100, 1));

		m_pShaderProgram->setMat4(model, "modelMatrix");
		m_pShaderProgram->setInt(layer, "layer");
		m_pShaderProgram->setFloat(0, "tex");

		glActiveTexture(GL_TEXTURE0);
		m_pTexture->bind();

		glBindVertexArray(m_VAO);
		
		m_vertexBuffer.bind();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

		m_pShaderProgram->use();
		
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}
}