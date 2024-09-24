#pragma once

#include <glad/glad.h>

namespace RenderEngine
{
	class Texture2D
	{
	public:

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

	private:

		GLuint m_ID;

		unsigned int m_width, m_height;
		GLenum m_mode;
	};
}