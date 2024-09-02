#include "Shader.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include "Renderer.h"

Shader::Shader(const std::string& vertexShader, const std::string& fragmentShader)
	: VerteFilePath(vertexShader), FragmentFilePath(fragmentShader)
{
	std::string vertexSource = ParseShader(vertexShader);
	std::string fragmentSource = ParseShader(fragmentShader);

	ShaderID = CreateShader(vertexSource, fragmentSource);
	
}

Shader::~Shader()
{
	glDeleteProgram(ShaderID);
}

void Shader::Bind() const
{
	glUseProgram(ShaderID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

std::string Shader::ParseShader(const std::string& filePath)
{
	std::ifstream stream(filePath);
	if (!stream.is_open())
	{
		std::cerr << "Could not open file: " << filePath << std::endl;
		return "";
	}

	std::string line;
	std::stringstream ss;

	while (getline(stream, line))
	{
		ss << line << '\n';
	}

	return ss.str();
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int success;
	char infoLog[512];
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(id, 512, nullptr, infoLog);
		std::cerr << "Error compiling shader: " << infoLog << std::endl;
		glDeleteShader(id);
		return 0;
	}
	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (UniformLocationCache.find(name) != UniformLocationCache.end())
	{
		return UniformLocationCache[name];
	}
	else
	{
		int location = glGetUniformLocation(ShaderID, name.c_str());

		if (location == -1)
		{
			std::cerr << "Warning: uniform " << name << " doesn't exist!" << std::endl;
			return -1;
		}
		else
		{
			UniformLocationCache[name] = location;
		}
		return location;
	}
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	int location = GetUniformLocation(name);
	if (location != -1)
	{
		glUniform1i(location, value);
	}
	else
	{
		std::cerr << "Error: uniform " << name << " doesn't exist!" << std::endl;
	}
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	int location = GetUniformLocation(name);
	if (location != -1)
	{
		glUniform4f(location, v0, v1, v2, v3);
	}
	else
	{
		std::cerr << "Error: uniform " << name << " doesn't exist!" << std::endl;
	}
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
	int location = GetUniformLocation(name);
	if (location != -1)
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
	}
	else
	{
		std::cerr << "Error: uniform " << name << " doesn't exist!" << std::endl;
	}
}

void Shader::SetUniformVec3f(const std::string& name, const glm::vec3& vector)
{
	int location = GetUniformLocation(name);
	if (location != -1)
	{
		glUniform3f(location, vector.x, vector.y, vector.z);
	}
	else
	{
		std::cerr << "Error: uniform " << name << " doesn't exist!" << std::endl;
	}
}

void Shader::SetUniformVec4f(const std::string& name, const glm::vec4& vector)
{
	int location = GetUniformLocation(name);
	if (location != -1)
	{
		glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
	}
	else
	{
		std::cerr << "Error: uniform " << name << " doesn't exist!" << std::endl;
	}
}