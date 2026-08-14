#include "SceneManager.h"
#include <glad/glad.h>
#include "ResourceManager.h"
#include "InputManager.h"
#include <fstream>
#include <sstream>
#include "../utils/Log.h"


// =================================================


void nothing::SceneManager::Init(EngineContext& ctx)
{

	ctx_ = &ctx;

}


// =================================================


void nothing::SceneManager::Update()
{

	static glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	static float cameraSpeed = 0.05f;


	auto& cam = GetCompFromCtx<Camera>();


#pragma region INPUT


	if (ctx_->inputManager->IsActionHeld(GameAction::MoveForward))
	{

		cam.position += cam.speed * cam.rotation;

	}


	if (ctx_->inputManager->IsActionHeld(GameAction::MoveBackward))
	{

		cam.position -= cam.speed * cam.rotation;

	}


	if (ctx_->inputManager->IsActionHeld(GameAction::MoveLeft))
	{

		cam.position += cam.speed * -glm::normalize(glm::cross(cam.rotation, up));

	}


	if (ctx_->inputManager->IsActionHeld(GameAction::MoveRight))
	{

		cam.position += cam.speed * glm::normalize(glm::cross(cam.rotation, up));

	}


	if (ctx_->inputManager->IsActionHeld(GameAction::RotateUp))
	{

		cam.rotation = glm::rotate(cam.rotation, glm::radians(cam.rotSens), glm::normalize(glm::cross(cam.rotation, up)));

	}


	if (ctx_->inputManager->IsActionHeld(GameAction::RotateDown))
	{

		cam.rotation = glm::rotate(cam.rotation, glm::radians(-cam.rotSens), glm::normalize(glm::cross(cam.rotation, up)));

	}


	if (ctx_->inputManager->IsActionHeld(GameAction::RotateLeft))
	{

		cam.rotation = glm::rotate(cam.rotation, glm::radians(cam.rotSens), up);

	}


	if (ctx_->inputManager->IsActionHeld(GameAction::RotateRight))
	{

		cam.rotation = glm::rotate(cam.rotation, glm::radians(-cam.rotSens), up);

	}


#pragma endregion

}


// =================================================


void nothing::SceneManager::Shutdown()
{

}


// =================================================


void nothing::SceneManager::LoadScene()
{

	// Esempio tattico nucleare
	std::string testMapPath = ctx_->filesystem->GetMapPath("testing");
	std::string mapFile = testMapPath + "\\testing.notmap";
	std::string assetFile = testMapPath + "\\assets.txt";


	std::vector<std::string> allTexturesFiles;
	std::vector<std::string> allModels3DFiles;
	std::vector<std::string> allAudioFiles;


	LoadAssetFile(assetFile.c_str(), allTexturesFiles, allModels3DFiles, allAudioFiles);


	for (auto& texFile : allTexturesFiles)
	{

		ctx_->resourcesManager->CreateTexture(ctx_->filesystem->GetTexturePathFromName(texFile));

	}


	// Questo serve per dopo, per caricare la texture del modello 3D durante la creazione del prop
	std::unordered_map<std::string, std::string> modelTextureMap;


	for (auto& modFile : allModels3DFiles)
	{

		ctx_->resourcesManager->CreateModel3D(ctx_->filesystem->GetModel3DPathFromName(modFile));


		std::string modName = modFile.erase(modFile.size() - 4); // Cancella ".obj"
		nothing::ResModel3D mod3D = ctx_->resourcesManager->GetModel3DFromName(modName);
		std::string modelTextureFileName = mod3D.modelTextureFileName;
		ctx_->resourcesManager->CreateTexture(ctx_->filesystem->GetTexturePathFromName(modelTextureFileName));
		modelTextureFileName.erase(modelTextureFileName.size() - 4); // Cancella ".png"
		modelTextureMap[modName] = modelTextureFileName;

	}


	std::fstream mapFileStream(mapFile, std::ios::in | std::ios::binary);


	if (mapFileStream.is_open())
	{

		char magic[4];
		mapFileStream.read(magic, 4);


		if (std::strncmp(magic, "NTGH", 4) == 0)
		{
			
			nothing::LogInfo("Valid magic number");

		}

		uint32_t version;
		mapFileStream.read(reinterpret_cast<char*>(&version), 4);


		if (version == 1)
		{

			nothing::LogInfo("Valid version: 1");

		}


		uint32_t objectsCount;
		mapFileStream.read(reinterpret_cast<char*>(&objectsCount), 4);


		if (objectsCount > 1000)
		{

			nothing::LogError("Too many objects in this map file, check for possible corruptions");

		}


		for (int i = 0; i < objectsCount; i++)
		{

			uint32_t objIDByte;
			mapFileStream.read(reinterpret_cast<char*>(&objIDByte), 4);


			if (objIDByte == 0x01)
			{

				float_t x, y, z, p, ya, r, w, h;
				mapFileStream.read(reinterpret_cast<char*>(&x), 4);
				mapFileStream.read(reinterpret_cast<char*>(&y), 4);
				mapFileStream.read(reinterpret_cast<char*>(&z), 4);
				mapFileStream.read(reinterpret_cast<char*>(&p), 4);
				mapFileStream.read(reinterpret_cast<char*>(&ya), 4);
				mapFileStream.read(reinterpret_cast<char*>(&r), 4);
				mapFileStream.read(reinterpret_cast<char*>(&w), 4);
				mapFileStream.read(reinterpret_cast<char*>(&h), 4);


				uint32_t texNameLenght;
				mapFileStream.read(reinterpret_cast<char*>(&texNameLenght), 4);


				std::string texName(texNameLenght, '\0');
				mapFileStream.read(texName.data(), texNameLenght);


				uint32_t isDoubleTiled;
				mapFileStream.read(reinterpret_cast<char*>(&isDoubleTiled), 4);


				SolidPlaneInfo spInfo{};
				spInfo.position      = glm::vec3(x, y, z);
				spInfo.rotation      = glm::vec3(p, ya, r);
				spInfo.width         = w;
				spInfo.height        = h;
				spInfo.textureID     = ctx_->resourcesManager->GetTextureIDFromName(texName);
				spInfo.isDoubleTiled = isDoubleTiled ? 1 : 0;


				CreateWorldSolidPlane(spInfo);
				
			}


			if (objIDByte == 0x02)
			{

				float_t x, y, z, p, ya, r;
				mapFileStream.read(reinterpret_cast<char*>(&x), 4);
				mapFileStream.read(reinterpret_cast<char*>(&y), 4);
				mapFileStream.read(reinterpret_cast<char*>(&z), 4);
				mapFileStream.read(reinterpret_cast<char*>(&p), 4);
				mapFileStream.read(reinterpret_cast<char*>(&ya), 4);
				mapFileStream.read(reinterpret_cast<char*>(&r), 4);


				uint32_t modNameLenght;
				mapFileStream.read(reinterpret_cast<char*>(&modNameLenght), 4);


				std::string modName(modNameLenght, '\0');
				mapFileStream.read(modName.data(), modNameLenght);


				uint32_t usePhysics;
				mapFileStream.read(reinterpret_cast<char*>(&usePhysics), 4);


				PropInfo propInfo{};
				propInfo.position          = glm::vec3(x, y, z);
				propInfo.rotation          = glm::vec3(p, ya, r);
				propInfo.modelVAO          = ctx_->resourcesManager->GetModel3DVAOFromName(modName);
				propInfo.modelIndicesCount = ctx_->resourcesManager->GetModel3DIndicesCountFromName(modName);
				propInfo.textureID         = ctx_->resourcesManager->GetTextureIDFromName(modelTextureMap[modName]);
				propInfo.usePhysics        = usePhysics ? 1 : 0;


				CreatePropObject(propInfo);

			}

		}


		mapFileStream.close();

	}


	registry.ctx().emplace<Camera>();

}


// =================================================


void nothing::SceneManager::UnloadScene()
{

	for (auto& wMesh : worldMeshes)
	{

		glDeleteVertexArrays(1, &wMesh.VAO);
		glDeleteBuffers(1, &wMesh.VBO);
		glDeleteBuffers(1, &wMesh.EBO);

	}


	worldMeshes.clear();
	registry.clear();
	registry.ctx().clear();

}


// =================================================


nothing::WorldMesh nothing::SceneManager::CreateCubeWorldMesh(float width, float height, float depth, bool isDoubleTiled)
{

	float W = width;
	float H = height;
	float D = depth;


	float tileScale = isDoubleTiled ? 0.5f : 1.0f;


	float vertices[] =
	{

		// FRONT (+Z)
		0.0f, 0.0f, D,     0.0f * tileScale, 0.0f * tileScale,
		W,    0.0f, D,     W    * tileScale, 0.0f * tileScale,
		W,    H,    D,     W    * tileScale, H    * tileScale,
		0.0f, H,    D,     0.0f * tileScale, H    * tileScale,

		// BACK (-Z)
		W,    0.0f, 0.0f,  0.0f * tileScale, 0.0f * tileScale,
		0.0f, 0.0f, 0.0f,  W    * tileScale, 0.0f * tileScale,
		0.0f, H,    0.0f,  W    * tileScale, H    * tileScale,
		W,    H,    0.0f,  0.0f * tileScale, H    * tileScale,

		// LEFT (-X)
		0.0f, 0.0f, 0.0f,  0.0f * tileScale, 0.0f * tileScale,
		0.0f, 0.0f, D,     D    * tileScale, 0.0f * tileScale,
		0.0f, H,    D,     D    * tileScale, H    * tileScale,
		0.0f, H,    0.0f,  0.0f * tileScale, H    * tileScale,

		// RIGHT (+X)
		W,    0.0f, D,     0.0f * tileScale, 0.0f * tileScale,
		W,    0.0f, 0.0f,  D    * tileScale, 0.0f * tileScale,
		W,    H,    0.0f,  D    * tileScale, H    * tileScale,
		W,    H,    D,     0.0f * tileScale, H    * tileScale,

		// TOP (+Y)
		0.0f, H,    D,     0.0f * tileScale, 0.0f * tileScale,
		W,    H,    D,     W    * tileScale, 0.0f * tileScale,
		W,    H,    0.0f,  W    * tileScale, D    * tileScale,
		0.0f, H,    0.0f,  0.0f * tileScale, D    * tileScale,

		// BOTTOM (-Y)
		0.0f, 0.0f, 0.0f,  0.0f * tileScale, 0.0f * tileScale,
		W,    0.0f, 0.0f,  W    * tileScale, 0.0f * tileScale,
		W,    0.0f, D,     W    * tileScale, D    * tileScale,
		0.0f, 0.0f, D,     0.0f * tileScale, D    * tileScale

	};


	unsigned int indices[] =
	{

		0, 1, 2,  2, 3, 0,        // front
		4, 5, 6,  6, 7, 4,        // back
		8, 9, 10, 10, 11, 8,      // left
		12, 13, 14, 14, 15, 12,   // right
		16, 17, 18, 18, 19, 16,   // top
		20, 21, 22, 22, 23, 20    // bottom

	};


	WorldMesh res{};


	glGenVertexArrays(1, &res.VAO);
	glGenBuffers(1, &res.VBO);
	glGenBuffers(1, &res.EBO);
	glBindVertexArray(res.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, res.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, res.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	// Hardcoded per ora
	res.numIndices = 36;


	return res;

}


// =================================================


nothing::WorldMesh nothing::SceneManager::CreatePlaneWorldMesh(float width, float height, bool isDoubleTiled)
{

	float W = width;
	float H = height;


	float tileScale = isDoubleTiled ? 0.5f : 1.0f;


	float vertices[] =
	{

		// FRONT (+Z)
		0.0f, 0.0f, 0.0f,     0.0f * tileScale, 0.0f * tileScale,
		W,    0.0f, 0.0f,     W    * tileScale, 0.0f * tileScale,
		W,    H,    0.0f,     W    * tileScale, H    * tileScale,
		0.0f, H,    0.0f,     0.0f * tileScale, H    * tileScale

	};


	unsigned int indices[] =
	{

		0, 1, 2,  2, 3, 0

	};


	WorldMesh res{};


	glGenVertexArrays(1, &res.VAO);
	glGenBuffers(1, &res.VBO);
	glGenBuffers(1, &res.EBO);
	glBindVertexArray(res.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, res.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, res.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	res.numIndices = 6;


	return res;

}


// =================================================


void nothing::SceneManager::CreateWorldSolidCube(const SolidCubeInfo& cubeInfo)
{

	WorldMesh m = CreateCubeWorldMesh(cubeInfo.width, cubeInfo.height, cubeInfo.depth, cubeInfo.isDoubleTiled);
	m.texture = cubeInfo.textureID;
	worldMeshes.emplace_back(m);


	auto cubeEnt = registry.create();
	registry.emplace<Object3D>(cubeEnt, worldMeshes.back().VAO, worldMeshes.back().numIndices, cubeInfo.textureID);
	registry.emplace<Transform>(cubeEnt, cubeInfo.position, cubeInfo.rotation);

}


// =================================================


void nothing::SceneManager::CreateWorldSolidPlane(const SolidPlaneInfo& planeInfo)
{

	WorldMesh m = CreatePlaneWorldMesh(planeInfo.width, planeInfo.height, planeInfo.isDoubleTiled);
	m.texture = planeInfo.textureID;
	worldMeshes.emplace_back(m);


	auto planeEnt = registry.create();
	registry.emplace<Object3D>(planeEnt, worldMeshes.back().VAO, worldMeshes.back().numIndices, planeInfo.textureID);
	registry.emplace<Transform>(planeEnt, planeInfo.position, planeInfo.rotation);

}


// =================================================


void nothing::SceneManager::CreatePropObject(const PropInfo& propInfo)
{

	auto propEnt = registry.create();
	registry.emplace<Object3D>(propEnt, propInfo.modelVAO, propInfo.modelIndicesCount, propInfo.textureID);
	registry.emplace<Transform>(propEnt, propInfo.position, propInfo.rotation);

}


// =================================================


bool nothing::SceneManager::LoadAssetFile(const char* assetFile, std::vector<std::string>& allTexturesFiles, std::vector<std::string>& allModels3DFiles, std::vector<std::string>& allAudioFiles)
{

	std::fstream testAssetsFile(assetFile, std::ios::in);


	if (testAssetsFile.is_open())
	{

		std::string line;


		int sectionToCheck = 0; // 0=Textures, 1=Modelli 3D, 2=Audio


		while (std::getline(testAssetsFile, line))
		{

			if (line == "_TEXTURES")
			{

				sectionToCheck = 0;

			}
			else if (line == "_MODELS3D")
			{

				sectionToCheck = 1;

			}
			else if (line == "_AUDIOS")
			{

				sectionToCheck = 2;

			}
			else
			{

				switch (sectionToCheck)
				{

				case 0:
					allTexturesFiles.push_back(line);
					break;


				case 1:
					allModels3DFiles.push_back(line);
					break;


				case 2:
					allAudioFiles.push_back(line);
					break;


				default:
					break;

				}

			}

		}


#pragma region CHECK ASSETS
		nothing::LogInfo("All Textures evaluated");


		for (auto tex : allTexturesFiles)
		{

			nothing::LogInfo(tex);

		}


		nothing::LogInfo("All 3D Models evaluated");


		for (auto mod : allModels3DFiles)
		{

			nothing::LogInfo(mod);

		}


		nothing::LogInfo("All Audios evaluated");


		for (auto aud : allAudioFiles)
		{

			nothing::LogInfo(aud);

		}
#pragma endregion

	}


	return true;

}


// =================================================