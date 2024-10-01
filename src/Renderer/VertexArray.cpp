#include "VertexArray.h"

namespace RenderEngine
{
	VertexArray::VertexArray() : m_elementsCount(0)
	{
		glGenVertexArrays(1, &m_ID);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_ID);
	}

	VertexArray& VertexArray::operator=(VertexArray&& array) noexcept
	{
		if (this != &array)
		{
			m_ID = array.m_ID;
			array.m_ID = 0;
			m_elementsCount = array.m_elementsCount;
			array.m_elementsCount = 0;
		}
		return *this;
	}

	VertexArray::VertexArray(VertexArray&& array) noexcept
	{
		m_ID = array.m_ID;
		array.m_ID = 0;
		m_elementsCount = array.m_elementsCount;
		array.m_elementsCount = 0;
	}

	void VertexArray::addBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout)
	{
		glBindVertexArray(m_ID);
		vertexBuffer.bind();
		const auto& layoutElements = layout.getLayoutElements();
		GLbyte* offset = nullptr;
		for (unsigned int i = 0; i < layoutElements.size(); ++i)
		{
			const auto& currentLayoutElement = layoutElements[i];
			GLuint currentAttribIndex = m_elementsCount + i;
			glEnableVertexAttribArray(currentAttribIndex);
			glVertexAttribPointer(currentAttribIndex, currentLayoutElement.count, currentLayoutElement.type, currentLayoutElement.normalized, layout.getStride(), offset);
			offset += currentLayoutElement.size;
		}
		m_elementsCount += static_cast<unsigned int>(layoutElements.size());
	}

	void VertexArray::bind() const
	{
		glBindVertexArray(m_ID);
	}

	void VertexArray::unbind() const
	{
		glBindVertexArray(0);
	}
}