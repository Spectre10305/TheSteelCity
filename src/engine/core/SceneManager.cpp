#include "SceneManager.h"
#include <glad/glad.h>
#include "ResourceManager.h"
#include "InputManager.h"


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


	auto view = registry.view<Transform>();


	for (auto [ent, tr] : view.each())
	{

		if (registry.all_of<DoNotMoveTag>(ent))
		{

			continue;

		}


		tr.rotation += 0.1f;

	}

}


// =================================================


void nothing::SceneManager::Shutdown()
{

}

void nothing::SceneManager::LoadScene()
{

	ctx_->resourcesManager->CreateTexture("D:\\TheSteelCity\\assets\\game\\textures\\tex_wall_bricks_1.png");
	ctx_->resourcesManager->CreateTexture("D:\\TheSteelCity\\assets\\game\\textures\\tex_floor_wood_1.png");


	SolidCubeInfo scInfo{};
	scInfo.position = glm::vec3(0.0f, 0.0f, 0.0f);
	scInfo.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	scInfo.width = 1.0f;
	scInfo.height = 1.0f;
	scInfo.depth = 1.0f;
	scInfo.textureID = ctx_->resourcesManager->GetTextureIDFromName("tex_floor_wood_1");
	scInfo.isDoubleTiled = true;
	scInfo.doNotMove = true;


	CreateWorldSolidCube(scInfo);



	SolidPlaneInfo spInfo{};
	spInfo.position = glm::vec3(2.0f, 0.0f, 0.0f);
	spInfo.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	spInfo.width = 1.0f;
	spInfo.height = 1.0f;
	spInfo.textureID = ctx_->resourcesManager->GetTextureIDFromName("tex_wall_bricks_1");
	spInfo.isDoubleTiled = true;
	spInfo.doNotMove = true;


	CreateWorldSolidPlane(spInfo);


	registry.ctx().emplace<Camera>();

}

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


	if (cubeInfo.doNotMove)
	{

		registry.emplace<DoNotMoveTag>(cubeEnt);

	}

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


	if (planeInfo.doNotMove)
	{

		registry.emplace<DoNotMoveTag>(planeEnt);

	}

}


// =================================================