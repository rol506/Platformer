#pragma once

#include <glad/glad.h>
#include <glm/vec2.hpp>

#include <string>
#include <map>

namespace RenderEngine
{
	class Texture2D
	{
	public:

		struct SubTexture2D
		{
			glm::vec2 leftBottomUV;
			glm::vec2 rightTopUV;

			SubTexture2D(const glm::vec2& _leftBottomUV, const glm::vec2& _rightTopUV) : leftBottomUV(_leftBottomUV), rightTopUV(_rightTopUV) {}
			SubTexture2D() : leftBottomUV(0.0f), rightTopUV(1.0f) {}
		};

		Texture2D(const int width, const int height, const unsigned int channels, const unsigned char* data, 
			      const GLenum filter=GL_LINEAR, const GLenum wrapMode=GL_CLAMP_TO_EDGE);
		~Texture2D();

		Texture2D(const Texture2D&) = delete;
		Texture2D& operator=(const Texture2D&) = delete;
		Texture2D& operator=(Texture2D&& texture) noexcept;
		Texture2D(Texture2D&& texture) noexcept;

		int width() const { return m_width; }
		int height() const { return m_height; }
		void bind() const;

		void addSubTexture(std::string& name, const glm::vec2& leftBottomUV, const glm::vec2& rightTopUV);
		const SubTexture2D& getSubTexture(const std::string& name) const;

	private:

		GLuint m_ID;

		unsigned int m_width, m_height;
		GLenum m_mode;

		std::map<std::string, SubTexture2D> m_subTextures;
	};
}