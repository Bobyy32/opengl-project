#include "Mesh.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"


Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	setupMesh();
}

void Mesh::Draw(Shader& shader, Camera& camera)
{
	shader.Bind();
	va.Bind();

	unsigned int diffuse = 0;
	unsigned int specular = 0;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		
	}
}

void Mesh::setupMesh()
{
	VertexBuffer vb(&vertices[0], vertices.size() * sizeof(Vertex));
	VertexBufferLayout layout;
	//Vertex positions
	layout.Push<float>(3);
	//Vertex normals
	layout.Push<float>(3);
	//Vertex texture coordinates
	layout.Push<float>(2);
	va.AddBuffer(vb, layout);
	IndexBuffer ib(&indices[0], indices.size());

	va.Unbind();
	vb.Unbind();
	ib.Unbind();
}

