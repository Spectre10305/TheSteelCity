#include "ResourceManager.h"
#include <glad/glad.h>
#include <stb_image.h>
#include <filesystem>
#include "../utils/Log.h"


// =================================================


void nothing::ResourceManager::InitDefaults()
{

	if (!CreateTexture("D:\\TheSteelCity\\assets\\game\\textures\\nothing_logo.png"))
	{

		nothing::LogWarning("Can't initialize default texture. Some visuals may be broken");

	}

}


// =================================================


// Restituisce VERO se la texture è stata creata, FALSO se non è stata creata oppure esiste già
bool nothing::ResourceManager::CreateTexture(const char* filePath)
{

	// Prende solo il nome del file.
	// Es: "D:\TheSteelCity\..\tex.png" -> "tex"
	auto fp = std::filesystem::path(filePath);
	std::string texName = fp.stem().string();


	if (allTextures_.find(texName) != allTextures_.end())
	{

		// La texture esiste già
		nothing::LogInfo("Texture: " + texName + " already exists.");
		return false;

	}


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

		nothing::LogError("Failed to load texture: " + texName);
		return false;

	}


	stbi_image_free(data);


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
	else
	{

		// Texture di default
		nothing::LogWarning("GetTextureFromName can't find texture: " + std::string(texName) + ". Using default texture");
		auto& defTex = allTextures_.at("nothing_logo");
		return defTex;

	}

}


// =================================================


uint32_t nothing::ResourceManager::GetTextureIDFromName(const char* texName)
{

	if (allTextures_.find(texName) != allTextures_.end())
	{

		return allTextures_.at(texName).id;

	}
	else
	{

		// Texture di default
		nothing::LogWarning("GetTextureIDFromName can't find texture: " + std::string(texName) + ". Using default texture");
		return allTextures_.at("nothing_logo").id;

	}

}


// =================================================