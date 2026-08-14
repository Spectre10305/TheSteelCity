#include "ResourceManager.h"
#include <glad/glad.h>
#include <stb_image.h>
#include <filesystem>
#include <tiny_obj_loader.h>
#include <vector>
#include "../utils/Log.h"


// =================================================


void nothing::ResourceManager::InitDefaults(EngineContext& ctx)
{

	ctx_ = &ctx;


	if (!CreateTexture(ctx_->filesystem->GetTexturePathFromName("nothing_logo.png")))
	{

		nothing::LogWarning("Can't initialize default texture. Some visuals may be broken");

	}


	if (!CreateModel3D(ctx_->filesystem->GetModel3DPathFromName("props_exterior_fence_1.obj")))
	{

		nothing::LogWarning("Can't initialize default 3D model. Some visuals may be broken");

	}

}


// =================================================


// Restituisce VERO se la texture è stata creata, FALSO se non è stata creata oppure esiste già
bool nothing::ResourceManager::CreateTexture(const std::string& filePath)
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
	unsigned char* data = stbi_load(filePath.c_str(), &res.width, &res.height, &res.channels, 0);


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


nothing::ResTexture& nothing::ResourceManager::GetTextureFromName(const std::string& texName)
{
	
	if (allTextures_.find(texName) != allTextures_.end())
	{

		auto& tex = allTextures_.at(texName);
		return tex;

	}
	else
	{

		// Texture di default
		nothing::LogWarning("GetTextureFromName can't find texture: " + std::string(texName) + ". Using default texture.");
		auto& defTex = allTextures_.at("nothing_logo");
		return defTex;

	}

}


// =================================================


uint32_t nothing::ResourceManager::GetTextureIDFromName(const std::string& texName)
{

	if (allTextures_.find(texName) != allTextures_.end())
	{

		return allTextures_.at(texName).id;

	}
	else
	{

		// Texture di default
		nothing::LogWarning("GetTextureIDFromName can't find texture: " + std::string(texName) + ". Using default texture.");
		return allTextures_.at("nothing_logo").id;

	}

}


// =================================================


void nothing::ResourceManager::DeleteAllTextures()
{

	for (auto& [texName, resTex] : allTextures_)
	{

		glDeleteTextures(1, &resTex.id);

	}


	allTextures_.clear();

}


// =================================================


bool nothing::ResourceManager::CreateModel3D(const std::string& filePath)
{

	auto fp = std::filesystem::path(filePath);
	std::string modName = fp.stem().string();


	if (allModels3D_.find(modName) != allModels3D_.end())
	{

		// Il Modello 3D
		nothing::LogInfo("3D Model: " + modName + " already exists.");
		return false;

	}


	tinyobj::ObjReaderConfig readerConfig;
	readerConfig.mtl_search_path = ""; // Prova a trovare il file .mtl nella stessa cartella del .obj


	tinyobj::ObjReader objReader;


	if (!objReader.ParseFromFile(filePath, readerConfig))
	{

		if (!objReader.Error().empty())
		{

			nothing::LogError("TinyObjLoader ERROR: " + objReader.Error());
		
		}


		return false;

	}


	// Controllo warning
	if (!objReader.Warning().empty())
	{

		nothing::LogWarning("Tinyobjloader warning: " + objReader.Warning());

	}


	const auto& attrib =    objReader.GetAttrib();
	const auto& shapes =    objReader.GetShapes();
	const auto& materials = objReader.GetMaterials();


	std::vector<nothing::Vertex3D> meshVertices;
	std::vector<uint32_t> meshIndices;


	for (const auto& shape : shapes)
	{

		for (const auto& idx : shape.mesh.indices)
		{

			nothing::Vertex3D v{};


			v.position =
			{

				attrib.vertices[3 * idx.vertex_index + 0],
				attrib.vertices[3 * idx.vertex_index + 1],
				attrib.vertices[3 * idx.vertex_index + 2]

			};


			if (idx.normal_index >= 0)
			{

				/*
				v.normal =
				{

					attrib.normals[3 * idx.normal_index + 0],
					attrib.normals[3 * idx.normal_index + 1],
					attrib.normals[3 * idx.normal_index + 2]

				};
				*/

			}


			if (idx.texcoord_index >= 0)
			{

				v.texCoords =
				{

					attrib.texcoords[2 * idx.texcoord_index + 0],
					attrib.texcoords[2 * idx.texcoord_index + 1]

				};

			}



			meshVertices.push_back(v);
			meshIndices.push_back(meshIndices.size());

		}

	}


	std::string modelTextureName;


	// Materiali (generalmente solo texture)
	for (const auto& material : materials)
	{

		if (!material.diffuse_texname.empty())
		{

			// Ricostruisce il nome della texture
			auto modelTexturePath = std::filesystem::path(material.diffuse_texname);
			std::string modelTextureNameOnly = modelTexturePath.stem().string();
			modelTextureName = modelTextureNameOnly + ".png";


			//std::string fullPath = ctx_->filesystem->GetTexturePathFromName(modelTextureFileName.c_str());

		}

	}


	ResModel3D res{};
	

	res.modelTextureFileName = modelTextureName;

	
	glGenVertexArrays(1, &res.vao);
	glGenBuffers(1, &res.vbo);
	glGenBuffers(1, &res.ebo);


	glBindVertexArray(res.vao);


	glBindBuffer(GL_ARRAY_BUFFER, res.vbo);
	glBufferData(GL_ARRAY_BUFFER, meshVertices.size() * sizeof(Vertex3D), meshVertices.data(), GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, res.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshIndices.size() * sizeof(uint32_t), meshIndices.data(), GL_STATIC_DRAW);


	// Posizione
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, position));
	glEnableVertexAttribArray(0);


	// Normale
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, texCoords));
	glEnableVertexAttribArray(1);


	// TexCoords
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, texCoords));
	//glEnableVertexAttribArray(2);


	glBindVertexArray(0);


	res.indicesCount = static_cast<uint32_t>(meshIndices.size());


	allModels3D_.emplace(modName, res);


	return true;

}


// =================================================


nothing::ResModel3D& nothing::ResourceManager::GetModel3DFromName(const std::string& modName)
{

	if (allModels3D_.find(modName) != allModels3D_.end())
	{

		auto& mod = allModels3D_.at(modName);
		return mod;

	}
	else
	{

		// Modello 3D di default
		nothing::LogWarning("GetModel3DFromName can't find 3D model: " + std::string(modName) + ". Using default model.");
		auto& defMod = allModels3D_.at("props_exterior_fence_1");
		return defMod;

	}

}


// =================================================


uint32_t nothing::ResourceManager::GetModel3DVAOFromName(const std::string& modName)
{

	if (allModels3D_.find(modName) != allModels3D_.end())
	{

		return allModels3D_.at(modName).vao;

	}
	else
	{

		nothing::LogWarning("GetModel3DVAOFromName can't find 3D model: " + std::string(modName) + ". Using default model.");
		return allModels3D_.at("props_exterior_fence_1").vao;

	}

}


// =================================================


uint32_t nothing::ResourceManager::GetModel3DIndicesCountFromName(const std::string& modName)
{

	if (allModels3D_.find(modName) != allModels3D_.end())
	{

		return allModels3D_.at(modName).indicesCount;

	}
	else
	{

		nothing::LogWarning("GetModel3DIndicesCountFromName can't find 3D model: " + std::string(modName) + ". Using default model.");
		return allModels3D_.at("props_exterior_fence_1").indicesCount;

	}

}


// =================================================


std::string nothing::ResourceManager::GetModel3DTextureFileName(const std::string& modName)
{

	if (allModels3D_.find(modName) != allModels3D_.end())
	{

		return allModels3D_.at(modName).modelTextureFileName;

	}
	else
	{

		nothing::LogWarning("GetModel3DTextureFileName can't find 3D model: " + std::string(modName) + ". Using default model's texture.");
		return allModels3D_.at("props_exterior_fence_1").modelTextureFileName;

	}

}


// =================================================


void nothing::ResourceManager::DeleteAllModels3D()
{

	for (auto& [modName, resMod] : allModels3D_)
	{

		glDeleteVertexArrays(1, &resMod.vao);
		glDeleteBuffers(1, &resMod.vbo);
		glDeleteBuffers(1, &resMod.ebo);

	}


	allModels3D_.clear();

}


// =================================================