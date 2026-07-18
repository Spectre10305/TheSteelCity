#pragma once
#include <vector>
#include <entt.hpp>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include "EngineContext.h"


namespace nothing
{

	// TEST Componenti
	struct Object3D
	{

		uint32_t meshVAO     = 0;
		uint32_t numVertices = 0;
		uint32_t textureID   = 0;

	};


	struct Transform
	{

		glm::vec3 position    = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 rotation    = glm::vec3(0.0f, 0.0f, 0.0f);

	};


	struct Camera
	{

		glm::vec3 position         = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 rotation         = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::mat4 viewMatrix       = glm::mat4(1.0f);
		glm::mat4 projectionMatrix = glm::mat4(1.0f);


		float speed = 0.05f;
		float rotSens = 1.0f;

	};


	struct DoNotMoveTag{};


	// TEST "InfoObjects", strutture temporanee per la creazione delle entità EnTT
	struct SolidCubeInfo
	{

		glm::vec3   position      = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3   rotation      = glm::vec3(0.0f, 0.0f, 0.0f);
		float       width         = 1.0f;
		float       height        = 1.0f;
		float       depth         = 1.0f;
		uint32_t    textureID     = 0;
		bool        isDoubleTiled = false;
		bool        doNotMove     = false;

	};


	struct SolidPlaneInfo
	{

		glm::vec3   position      = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3   rotation      = glm::vec3(0.0f, 0.0f, 0.0f);
		float       width         = 1.0f;
		float       height        = 1.0f;
		uint32_t    textureID     = 0;
		bool        isDoubleTiled = false;
		bool        doNotMove     = false;

	};


	// Queste sono quelle mesh generate per il mondo come cubi o piani
	struct WorldMesh
	{

		uint32_t VAO        = 0;
		uint32_t VBO        = 0;
		uint32_t EBO        = 0;
		uint32_t numIndices = 0;
		uint32_t texture    = 0;

	};


	class SceneManager
	{

	public:

		void Init(EngineContext& ctx);
		void Update();
		void Shutdown();


		WorldMesh CreateCubeWorldMesh(float width, float height, float depth, bool isDoubleTiled);
		WorldMesh CreatePlaneWorldMesh(float width, float height, bool isDoubleTiled);
		void CreateWorldSolidCube(const SolidCubeInfo& cubeInfo);
		void CreateWorldSolidPlane(const SolidPlaneInfo& planeInfo);


		template<typename T>
		T& GetCompFromCtx()
		{

			T& res = registry.ctx().get<T>();
			return res;

		}


		entt::registry registry;


		std::vector<WorldMesh> worldMeshes;


	private:

		EngineContext* ctx_ = nullptr;

	};

}