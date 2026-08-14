#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
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


	struct ResModel3D
	{

		uint32_t    vao;
		uint32_t    vbo;
		uint32_t    ebo;
		uint32_t    indicesCount;
		std::string modelTextureFileName; // Scritto come "texture.png"

	};


	// Helper per caricare i modelli 3D
	struct Vertex3D
	{

		glm::vec3 position;
		//glm::vec3 normal;
		glm::vec2 texCoords;

	};


	// SCOPO: Gestione risorse di gioco (textures, modelli 3D, audio...)
	class ResourceManager
	{

	public:

		void InitDefaults(EngineContext& ctx);


		// Textures
		bool        CreateTexture(const std::string& filePath);
		ResTexture& GetTextureFromName(const std::string& texName);
		uint32_t    GetTextureIDFromName(const std::string& texName);
		void        DeleteAllTextures();


		// Modelli 3D
		bool        CreateModel3D(const std::string& filePath);
		ResModel3D& GetModel3DFromName(const std::string& modName);
		uint32_t    GetModel3DVAOFromName(const std::string& modName);
		uint32_t    GetModel3DIndicesCountFromName(const std::string& modeName);
		std::string GetModel3DTextureFileName(const std::string& modName);
		void        DeleteAllModels3D();

	private:

		std::unordered_map<std::string, ResTexture> allTextures_;
		std::unordered_map<std::string, ResModel3D> allModels3D_;


		EngineContext* ctx_ = nullptr;

	};

}