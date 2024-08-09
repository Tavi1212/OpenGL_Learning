#include "Texture.h"
#include "Renderer.h"
#include "vendor/stb_image/stb_image.h"

#include <GL/glew.h>
#include <iostream>

Texture::Texture(const std::string& path)
	:m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr),
	 m_Width(0), m_Height(0), m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D,m_RendererID));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));

	// Configure Sampler
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D,0));

	if (!m_LocalBuffer)
	{
		std::cout << "Failed to load texture: " << path << std::endl;
		std::cout << "Reason: " << stbi_failure_reason() << std::endl;
	}
	else
	{
		std::cout << "Path: " << m_FilePath << std::endl;
		std::cout << "Height: " << m_Height << std::endl;
		std::cout << "Width: " << m_Width << std::endl;
		std::cout << "Bits per pixel : " << m_BPP << std::endl;
	}

	//GLCall(glGenTextures(1, &m_RendererID));
	//GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
