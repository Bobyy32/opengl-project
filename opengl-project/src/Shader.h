#pragma once

#include <string>

#include <glm/glm.hpp>
#include <unordered_map>


class Shader
{
private:
	unsigned int ShaderID;
	std::string VerteFilePath;
	std::string FragmentFilePath;

	std::unordered_map<std::string, int> UniformLocationCache;

public:
	Shader(const std::string& vertexShader, const std::string& fragmentShader);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform1i(const std::string& name, int value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
	void SetUniformVec3f(const std::string& name, const glm::vec3& vector);
	void SetUniformVec4f(const std::string& name, const glm::vec4& vector);

private:

	std::string ParseShader(const std::string& filePath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	int GetUniformLocation(const std::string& name);

};

