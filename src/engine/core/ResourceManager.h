#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>


namespace nothing
{

	struct ResTexture
	{

		uint32_t    id;
		std::string name;
		int32_t     width;
		int32_t     height;
		int32_t     channels;

	};


	// SCOPO: Gestione risorse di gioco (textures, modelli 3D, audio...)
	class ResourceManager
	{

	public:

		bool        CreateTexture(const char* filePath);
		ResTexture& GetTextureFromName(const char* texName);
		uint32_t    GetTextureIDFromName(const char* texName);

	private:

		std::unordered_map<std::string, ResTexture> allTextures_;

	};

}