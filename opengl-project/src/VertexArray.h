#pragma once

#include "VertexBuffer.h"

#include <GL/glew.h>

#include <vector>
#include <stdexcept>

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;
	unsigned int typeSize;
	
	VertexBufferElement(unsigned int t, unsigned int c, unsigned char n, unsigned int tS)
		: type(t), count(c), normalized(n), typeSize(tS) {}

};

class VertexBufferLayout
{
private:

	std::vector<VertexBufferElement> Elements;
	unsigned int Stride;

public:
	VertexBufferLayout() : Stride(0) {}

	template<typename T>
	void Push(unsigned int count)
	{
		throw std::runtime_error("Unsupported type for VertexBufferLayout::Push.");
	}

	template<>
	void Push<float>(unsigned int count)
	{
		Elements.push_back(VertexBufferElement(GL_FLOAT, count, GL_FALSE, 4));
		Stride += count * 4;
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		Elements.push_back(VertexBufferElement(GL_UNSIGNED_INT, count, GL_FALSE, 4));
		Stride += count * 4;
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		Elements.push_back(VertexBufferElement(GL_UNSIGNED_BYTE, count, GL_TRUE, 1));
		Stride += count * 1;
	}

	inline const std::vector<VertexBufferElement>& GetElements() const { return Elements; }
	inline unsigned int GetStride() const { return Stride; }

};

class VertexArray
{
private:
	unsigned int VertexArrayID;

public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;

};

