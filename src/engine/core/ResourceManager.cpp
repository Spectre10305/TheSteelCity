#include "ResourceManager.h"
#include <glad/glad.h>
#include <stb_image.h>
#include <filesystem>
#include "../utils/Log.h"


// =================================================


bool nothing::ResourceManager::CreateTexture(const char* filePath)
{

	ResTexture res{};


	glGenTextures(1, &res.id);
	glBindTexture(GL_TEXTURE_2D, res.id);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filePath, &res.width, &res.height, &res.channels, 0);


	if (data)
	{

		switch (res.channels)
		{

		case 3:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, res.width, res.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			break;


		case 4:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, res.width, res.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			break;


		default:
			// Fallback a RGB
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, res.width, res.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			break;

		}


		glGenerateMipmap(GL_TEXTURE_2D);

	}
	else
	{

		nothing::LogError("Failed to load texture");
		return false;

	}


	stbi_image_free(data);


	// Prende solo il nome del file.
	// Es: "D:\TheSteelCity\..\tex.png" -> "tex"
	auto fp = std::filesystem::path(filePath);
	std::string texName = fp.stem().string();


	allTextures_.emplace(texName, res);


	return true;

}


// =================================================


nothing::ResTexture& nothing::ResourceManager::GetTextureFromName(const char* texName)
{
	
	if (allTextures_.find(texName) != allTextures_.end())
	{

		auto& tex = allTextures_.at(texName);
		return tex;

	}

}


// =================================================


uint32_t nothing::ResourceManager::GetTextureIDFromName(const char* texName)
{

	if (allTextures_.find(texName) != allTextures_.end())
	{

		return allTextures_.at(texName).id;

	}

}


// =================================================