#pragma once

#include <vector>
#include <string>
#include <memory>

#include <glm/glm.hpp>

#include "VertexArray.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	VertexArray va;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void Draw(Shader& shader, Camera& camera);

private:
	void setupMesh();
};

