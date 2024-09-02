#pragma once

class Texture
{
private:
	unsigned int TextureID;
	unsigned char* data;
	int Width, Height, nrChannels;
						//number of color channels

public:
	Texture(const char* filePath);
	~Texture();

	void Bind(unsigned int slot) const;
	void Unbind() const;
};
