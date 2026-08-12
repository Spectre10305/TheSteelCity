#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include "EngineContext.h"
#include "Filesystem.h"


namespace nothing
{

	struct ResTexture
	{

		uint32_t    id;
		int32_t     width;
		int32_t     height;
		int32_t     channels;

	};


	// SCOPO: Gestione risorse di gioco (textures, modelli 3D, audio...)
	class ResourceManager
	{

	public:

		void InitDefaults(EngineContext& ctx);


		// Textures
		bool        CreateTexture(const char* filePath);
		ResTexture& GetTextureFromName(const char* texName);
		uint32_t    GetTextureIDFromName(const char* texName);
		void        DeleteAllTextures();

	private:

		std::unordered_map<std::string, ResTexture> allTextures_;


		EngineContext* ctx_ = nullptr;

	};

}