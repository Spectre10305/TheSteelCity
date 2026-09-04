#include "SceneManager.h"
#include <glad/glad.h>
#include "ResourceManager.h"
#include "InputManager.h"
#include <sstream>
#include "../utils/Log.h"
#include "../utils/TransformsUtils.h"
#include "../utils/RandomGen.h"
#include "../utils/MemCheck.h"
#include "../game/components/Object3D.h"
#include "../game/components/Transform.h"
#include "../game/components/Velocity.h"
#include "../game/components/Camera.h"
#include "../game/components/PlayerInput.h"
#include "../game/components/CustomBehaviour.h"
#include "../game/components/TestingComponent.h"
#include "../game/components/Tags.h"
#include "../game/components/NameTag.h"
#include "../game/components/PhysicsBody.h"
#include "../game/components/EntityReference.h"
#include "../game/custom_behaviours/CameraBehaviour.h"
#include "../game/custom_behaviours/PlayerBehaviour.h"
#include "../game/custom_behaviours/TestCustomBehaviour.h"


// =================================================


void nothing::SceneManager::Init(EngineContext& ctx)
{

	ctx_ = &ctx;
	engineServices_.PrintInfoMessage = nothing::LogInfo;

}


// =================================================


void nothing::SceneManager::Update(double deltaTime)
{

	auto& inp = GetCompFromCtx<components::PlayerInput>();


#pragma region INPUT

	inp.moveForward   = ctx_->inputManager->IsActionHeld(GameAction::MoveForward)  ? 1.0f : 0.0f;
	inp.moveBackwards = ctx_->inputManager->IsActionHeld(GameAction::MoveBackward) ? 1.0f : 0.0f;
	inp.moveLeft      = ctx_->inputManager->IsActionHeld(GameAction::MoveLeft)     ? 1.0f : 0.0f;
	inp.moveRight     = ctx_->inputManager->IsActionHeld(GameAction::MoveRight)    ? 1.0f : 0.0f;
	inp.rotateUp      = ctx_->inputManager->IsActionHeld(GameAction::RotateUp)     ? 1.0f : 0.0f;
	inp.rotateDown    = ctx_->inputManager->IsActionHeld(GameAction::RotateDown)   ? 1.0f : 0.0f;
	inp.rotateLeft    = ctx_->inputManager->IsActionHeld(GameAction::RotateLeft)   ? 1.0f : 0.0f;
	inp.rotateRight   = ctx_->inputManager->IsActionHeld(GameAction::RotateRight)  ? 1.0f : 0.0f;
	inp.running       = ctx_->inputManager->IsActionHeld(GameAction::Run)          ? 1.0f : 0.0f;


	ctx_->inputManager->GetMouseDelta(inp.mouseXDelta, inp.mouseYDelta);

#pragma endregion


	if (!ctx_->isGamePaused)
	{

		auto customBehView = registry.view<nothing::components::CustomBehaviour>();


		for (auto [ent, beh] : customBehView.each())
		{

			beh.customBehaviour->Update(deltaTime);
			beh.customBehaviour->LateUpdate(deltaTime);

		}

	}

}


// =================================================


void nothing::SceneManager::Shutdown()
{
	// ...
}


// =================================================


void nothing::SceneManager::LoadScene()
{

	using namespace nothing::components;


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

#pragma region notmap header


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


#pragma endregion


		for (int i = 0; i < objectsCount; i++)
		{

			uint32_t objIDByte;
			mapFileStream.read(reinterpret_cast<char*>(&objIDByte), 4);


			switch (objIDByte)
			{

			case 0x01:
				ReadPlaneDataFromFile(mapFileStream);
				break;


			case 0x02:
				ReadPropDataFromFile(mapFileStream, modelTextureMap);
				break;


			default:
				break;

			}

		}


		mapFileStream.close();

	}


	registry.ctx().emplace<components::PlayerInput>();


	CreatePlayer();




	// Test fisica

	
	//ctx_->resourcesManager->CreateTexture(ctx_->filesystem->GetTexturePathFromName("tex_wall_bricks_1.png"));
	SolidCubeInfo ground{};
	ground.position      = glm::vec3(-6.0f, -1.0f, -5.0f);
	ground.rotation      = glm::vec3(0.0f, 0.0f, 0.0f);
	ground.width         = 10.0f;
	ground.height        = 1.0f;
	ground.depth         = 10.0f;
	ground.textureID     = ctx_->resourcesManager->GetTextureIDFromName("nothing_logo");
	ground.usePhysics    = false;
	ground.isDoubleTiled = true;
	

	CreateWorldSolidCube(ground);


	ctx_->resourcesManager->CreateTexture(ctx_->filesystem->GetTexturePathFromName("tex_wall_bricks_1.png"));
	SolidCubeInfo column{};
	column.position = glm::vec3(0.0f, 0.0f, 4.0f);
	column.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	column.width =  1.0f;
	column.height = 2.0f;
	column.depth =  1.0f;
	column.textureID = ctx_->resourcesManager->GetTextureIDFromName("tex_wall_bricks_1");
	column.usePhysics = false;
	column.isDoubleTiled = false;


	CreateWorldSolidCube(column);


	// Test Trigger 1


	auto testLogicEnt = registry.create();
	auto testTriggerBeh = std::make_unique<TestCustomBehaviour>();
	testTriggerBeh->SetRegistry(registry);
	testTriggerBeh->SetEntity(testLogicEnt);
	registry.emplace<CustomBehaviour>(testLogicEnt, std::move(testTriggerBeh));


	auto triggerEnt = registry.create();
	registry.emplace<Transform>(triggerEnt, glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	registry.emplace<PhysicsBody>(triggerEnt, MeshType::Cube, BodyType::Static, 1.0f, 2.0f, 4.0f, false, true, triggerEnt);
	registry.emplace<EntityReference>(triggerEnt, testLogicEnt);


	/*
	for (int i = 0; i < 100; i++)
	{

		nothing::Random::RandomizeSeed();
		float randX = nothing::Random::Float(-5.0f, 5.0f);
		float randY = nothing::Random::Float(-5.0f, 5.0f);
		float randZ = nothing::Random::Float(-5.0f, 5.0f);


		SolidCubeInfo cubePhysTest{};
		cubePhysTest.position = glm::vec3(randX, randY + 10.0f, randZ);
		cubePhysTest.rotation = glm::vec3(randX, randY, randZ);
		cubePhysTest.width = 0.5f;
		cubePhysTest.height = 0.5f;
		cubePhysTest.depth = 0.5f;
		cubePhysTest.textureID = ctx_->resourcesManager->GetTextureIDFromName("tex_wall_bricks_1");
		cubePhysTest.usePhysics = true;
		cubePhysTest.isCentered = false;
		cubePhysTest.isDoubleTiled = true;


		CreateWorldSolidCube(cubePhysTest);

	}
	*/


	// Crea tutti i custom behaviours
	auto customBehView = registry.view<nothing::components::CustomBehaviour>();


	for (auto [ent, beh] : customBehView.each())
	{

		beh.customBehaviour->engineServices_ = &engineServices_;
		beh.customBehaviour->Create();

	}


	// Stampa l'uso della memoria degli asset
	nothing::InterrogateMemoryStatus();

}


// =================================================


void nothing::SceneManager::UnloadScene()
{

	auto customBehView = registry.view<nothing::components::CustomBehaviour>();


	for (auto [ent, beh] : customBehView.each())
	{

		beh.customBehaviour->Destroy();
		beh.customBehaviour.reset();

	}


	for (auto& wMesh : worldMeshes)
	{

		glDeleteVertexArrays(1, &wMesh.VAO);
		glDeleteBuffers(1, &wMesh.VBO);
		glDeleteBuffers(1, &wMesh.EBO);

	}


	worldMeshes.clear();
	registry.ctx().clear();
	registry.clear();

}


// MESH MONDO=======================================


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


nothing::WorldMesh nothing::SceneManager::CreateCenteredCubeWorldMesh(float width, float height, float depth, bool isDoubleTiled)
{

	float x = width * 0.5f;
	float y = height * 0.5f;
	float z = depth * 0.5f;


	float tileScale = isDoubleTiled ? 0.5f : 1.0f;


	float vertices[] =
	{

		// FRONT (+Z)
		-x, -y, +z,       0.0f * tileScale, 0.0f * tileScale,
		+x, -y, +z,       x    * tileScale, 0.0f * tileScale,
		+x, +y, +z,       x    * tileScale, y    * tileScale,
		-x, +y, +z,       0.0f * tileScale, y    * tileScale,


		// BACK (-Z)
		+x, -y, -z,       0.0f * tileScale, 0.0f * tileScale,
		-x, -y, -z,       x    * tileScale, 0.0f * tileScale,
		-x, +y, -z,       x    * tileScale, y    * tileScale,
		+x, +y, -z,       0.0f * tileScale, y    * tileScale,


		// LEFT (-X)
		-x, -y, -z,       0.0f * tileScale, 0.0f * tileScale,
		-x, -y, +z,       z    * tileScale, 0.0f * tileScale,
		-x, +y, +z,       z    * tileScale, y    * tileScale,
		-x, +y, -z,       0.0f * tileScale, y    * tileScale,


		// RIGHT (+X)
		+x, -y, +z,       0.0f * tileScale, 0.0f * tileScale,
		+x, -y, -z,       z    * tileScale, 0.0f * tileScale,
		+x, +y, -z,       z    * tileScale, y    * tileScale,
		+x, +y, +z,       0.0f * tileScale, y    * tileScale,


		// TOP (+Y)
		-x, +y, +z,       0.0f * tileScale, 0.0f * tileScale,
		+x, +y, +z,       x    * tileScale, 0.0f * tileScale,
		+x, +y, -z,       x    * tileScale, z    * tileScale,
		-x, +y, -z,       0.0f * tileScale, z    * tileScale,


		// BOTTOM (-Y)
		-x, -y, -z,       0.0f * tileScale, 0.0f * tileScale,
		+x, -y, -z,       x    * tileScale, 0.0f * tileScale,
		+x, -y, +z,       x    * tileScale, z    * tileScale,
		-x, -y, +z,       0.0f * tileScale, z    * tileScale

	};


	unsigned int indices[] =
	{

		
		0, 1, 2, 2, 3, 0,       // FRONT  (+Z)
		4, 5, 6, 6, 7, 4,       // BACK   (-Z)
		8, 9, 10, 10, 11, 8,    // LEFT   (-X)
		12, 13, 14, 14, 15, 12, // RIGHT  (+X)
		16, 17, 18, 18, 19, 16, // TOP    (+Y)
		20, 21, 22, 22, 23, 20  // BOTTOM (-Y)

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


// OGGETTI ECS MONDO================================


void nothing::SceneManager::CreateWorldSolidCube(const SolidCubeInfo& cubeInfo)
{

	WorldMesh m{};


	if (cubeInfo.isCentered)
	{

		m = CreateCenteredCubeWorldMesh(cubeInfo.width, cubeInfo.height, cubeInfo.depth, cubeInfo.isDoubleTiled);

	}
	else
	{

		m = CreateCubeWorldMesh(cubeInfo.width, cubeInfo.height, cubeInfo.depth, cubeInfo.isDoubleTiled);

	}


	m.texture = cubeInfo.textureID;
	worldMeshes.emplace_back(m);


	auto cubeEnt = registry.create();
	registry.emplace<components::Object3D>(cubeEnt, worldMeshes.back().VAO, worldMeshes.back().numIndices, cubeInfo.textureID);
	registry.emplace<components::Transform>(cubeEnt, cubeInfo.position, nothing::EulerToQuaternion(cubeInfo.rotation));


	nothing::components::BodyType physBodyType;


	if (cubeInfo.usePhysics)
	{

		physBodyType = nothing::components::BodyType::Dynamic;

	}
	else
	{

		physBodyType = nothing::components::BodyType::Static;

	}


	bool centered = cubeInfo.isCentered ? true : false;
	registry.emplace<components::PhysicsBody>(cubeEnt, nothing::components::MeshType::Cube, physBodyType, cubeInfo.width, cubeInfo.height, cubeInfo.depth, centered, false, cubeEnt);
	//registry.emplace<components::NameTag>(cubeEnt, "This is a Cube");

}


// =================================================


void nothing::SceneManager::CreateWorldSolidPlane(const SolidPlaneInfo& planeInfo)
{

	WorldMesh m = CreatePlaneWorldMesh(planeInfo.width, planeInfo.height, planeInfo.isDoubleTiled);
	m.texture = planeInfo.textureID;
	worldMeshes.emplace_back(m);


	auto planeEnt = registry.create();
	registry.emplace<components::Object3D>(planeEnt, worldMeshes.back().VAO, worldMeshes.back().numIndices, planeInfo.textureID);
	registry.emplace<components::Transform>(planeEnt, planeInfo.position, nothing::EulerToQuaternion(planeInfo.rotation));
	registry.emplace<components::PhysicsBody>(planeEnt, components::MeshType::Plane, components::BodyType::Static, planeInfo.width, planeInfo.height, 0.01f, false, false, planeEnt);

}


// =================================================


void nothing::SceneManager::CreatePropObject(const PropInfo& propInfo)
{

	auto propEnt = registry.create();
	registry.emplace<components::Object3D>(propEnt, propInfo.modelVAO, propInfo.modelIndicesCount, propInfo.textureID);
	registry.emplace<components::Transform>(propEnt, propInfo.position, nothing::EulerToQuaternion(propInfo.rotation));
	
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


void nothing::SceneManager::ReadPlaneDataFromFile(std::fstream& f)
{

	float_t x, y, z, p, ya, r, w, h;
	f.read(reinterpret_cast<char*>(&x), 4);
	f.read(reinterpret_cast<char*>(&y), 4);
	f.read(reinterpret_cast<char*>(&z), 4);
	f.read(reinterpret_cast<char*>(&p), 4);
	f.read(reinterpret_cast<char*>(&ya), 4);
	f.read(reinterpret_cast<char*>(&r), 4);
	f.read(reinterpret_cast<char*>(&w), 4);
	f.read(reinterpret_cast<char*>(&h), 4);


	uint32_t texNameLenght;
	f.read(reinterpret_cast<char*>(&texNameLenght), 4);


	std::string texName(texNameLenght, '\0');
	f.read(texName.data(), texNameLenght);


	uint32_t isDoubleTiled;
	f.read(reinterpret_cast<char*>(&isDoubleTiled), 4);


	SolidPlaneInfo spInfo{};
	spInfo.position = glm::vec3(x, y, z);
	spInfo.rotation = glm::vec3(p, ya, r);
	spInfo.width = w;
	spInfo.height = h;
	spInfo.textureID = ctx_->resourcesManager->GetTextureIDFromName(texName);
	spInfo.isDoubleTiled = isDoubleTiled ? 1 : 0;


	CreateWorldSolidPlane(spInfo);

}


// =================================================


void nothing::SceneManager::ReadPropDataFromFile(std::fstream& f, std::unordered_map<std::string, std::string>& modelTextureMap)
{

	float_t x, y, z, p, ya, r;
	f.read(reinterpret_cast<char*>(&x), 4);
	f.read(reinterpret_cast<char*>(&y), 4);
	f.read(reinterpret_cast<char*>(&z), 4);
	f.read(reinterpret_cast<char*>(&p), 4);
	f.read(reinterpret_cast<char*>(&ya), 4);
	f.read(reinterpret_cast<char*>(&r), 4);


	uint32_t modNameLenght;
	f.read(reinterpret_cast<char*>(&modNameLenght), 4);


	std::string modName(modNameLenght, '\0');
	f.read(modName.data(), modNameLenght);


	uint32_t usePhysics;
	f.read(reinterpret_cast<char*>(&usePhysics), 4);


	PropInfo propInfo{};
	propInfo.position = glm::vec3(x, y, z);
	propInfo.rotation = glm::vec3(p, ya, r);
	propInfo.modelVAO = ctx_->resourcesManager->GetModel3DVAOFromName(modName);
	propInfo.modelIndicesCount = ctx_->resourcesManager->GetModel3DIndicesCountFromName(modName);
	propInfo.textureID = ctx_->resourcesManager->GetTextureIDFromName(modelTextureMap[modName]);
	propInfo.usePhysics = usePhysics ? 1 : 0;


	CreatePropObject(propInfo);

}


// =================================================


void nothing::SceneManager::CreatePlayer()
{

	using namespace nothing::components;


	ctx_->resourcesManager->CreateModel3D(ctx_->filesystem->GetModel3DPathFromName("props_testing_marty.obj"));
	nothing::ResModel3D martyModRef = ctx_->resourcesManager->GetModel3DFromName("props_testing_marty");
	ctx_->resourcesManager->CreateTexture(ctx_->filesystem->GetTexturePathFromName(martyModRef.modelTextureFileName));


	glm::vec3 playerPosition = glm::vec3(6.0f, 0.0f, -1.0f);
	glm::vec3 playerRotation = glm::vec3(0.0f, 0.0f, 0.0f);


	auto playerEnt = registry.create();
	registry.emplace<Object3D>(playerEnt, martyModRef.vao, martyModRef.indicesCount, ctx_->resourcesManager->GetTextureIDFromName(martyModRef.modelTextureFileName.erase(martyModRef.modelTextureFileName.size() - 4)));
	registry.emplace<Transform>(playerEnt, playerPosition, nothing::EulerToQuaternion(playerRotation));
	registry.emplace<Velocity>(playerEnt, glm::vec3(0.0f, 0.0f, 0.0f));


	// Tutti valori di default, vengono ignorati alla creazione della fisica del player. Serve solo per impostare selfEntID
	registry.emplace<PhysicsBody>(playerEnt, MeshType::Cube, BodyType::Static, 1.0f, 1.0f, 1.0f, false, false, playerEnt);


	registry.emplace<Camera>(playerEnt, playerPosition, glm::vec3(0.0f, 0.0f, 0.0f));


	auto playerBeh = std::make_unique<nothing::PlayerBehaviour>();
	playerBeh->SetRegistry(registry);
	playerBeh->SetEntity(playerEnt);
	registry.emplace<CustomBehaviour>(playerEnt, std::move(playerBeh));


	registry.emplace<MainCameraTag>(playerEnt);
	registry.emplace<PlayerTag>(playerEnt);
	//registry.emplace<NameTag>(playerEnt, "Player");

}


// =================================================