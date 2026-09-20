#pragma once
#include <Windows.h>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <functional>
#include <entt.hpp>
#include <glm/glm.hpp>
#include "EngineContext.h"
#include "../game/basic/BaseCustomBehaviour.h" // EngineServices


namespace nothing
{

	// TEST "InfoObjects", strutture temporanee per la creazione delle entità EnTT
	struct SolidCubeInfo
	{

		uint64_t    ID            = 0;
		glm::vec3   position      = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3   rotation      = glm::vec3(0.0f, 0.0f, 0.0f);
		float       width         = 1.0f;
		float       height        = 1.0f;
		float       depth         = 1.0f;
		uint32_t    textureID     = 0;
		bool        isCentered    = false;
		bool        isDoubleTiled = false;
		bool        usePhysics    = false;
		float       density       = 1.0f;

		// NOTA: Di default il cubo parte da 0,0,0 e si estende verso
		// +X, +Y e +Z. "isCentered" estende il cubo in tutte le direzioni
		// allo stesso modo. Quindi un cubo con WIDTH 2.0 sarà 1.0 su +X e 1.0 su -X

	};


	struct SolidPlaneInfo
	{

		uint64_t    ID            = 0;
		glm::vec3   position      = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3   rotation      = glm::vec3(0.0f, 0.0f, 0.0f);
		float       width         = 1.0f;
		float       height        = 1.0f;
		uint32_t    textureID     = 0;
		bool        isDoubleTiled = false;

	};


	struct PropInfo
	{

		uint64_t    ID                = 0;
		glm::vec3   position          = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3   rotation          = glm::vec3(0.0f, 0.0f, 0.0f);
		uint32_t    modelVAO          = 0;
		uint32_t    modelIndicesCount = 0;
		uint32_t    textureID         = 0;
		bool        usePhysics        = false;

	};


	struct TriggerInfo
	{

		uint64_t    ID            = 0;
		glm::vec3   position      = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3   rotation      = glm::vec3(0.0f, 0.0f, 0.0f);
		float       width         = 1.0f;
		float       height        = 1.0f;
		float       depth         = 1.0f;
		uint64_t    targetEntiyID = 0;

	};


	// Sì, è un duplicato di TriggerInfo...
	struct InteractableInfo
	{

		uint64_t    ID            = 0;
		glm::vec3   position      = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3   rotation      = glm::vec3(0.0f, 0.0f, 0.0f);
		float       width         = 1.0f;
		float       height        = 1.0f;
		float       depth         = 1.0f;
		uint64_t    targetEntiyID = 0;

	};


	struct TestEntityInfo
	{

		uint64_t ID          = 0;
		float    garbageVal1 = 0.0f;
		float    garbageVal2 = 0.0f;
		float    garbageVal3 = 0.0f;

	};


	struct MoveObjectEntityInfo
	{

		uint64_t  ID              = 0;
		uint64_t  targetID        = 0;
		glm::vec3 desiredPosition = glm::vec3(0.0f, 0.0f, 0.0f);
		uint32_t  duration        = 1000;
		bool      ignoreY         = false;
		bool      twoWays         = false;

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
		void Update(double deltaTime);
		void Shutdown();


		void InitServices();


		// NOTA: "mapName" deve essere il solo nome della mappa, niente ".notmap" o percorsi
		void LoadScene(const std::string& mapName);
		void UnloadScene();
		entt::entity ResolveEntityID(uint64_t entID);


		WorldMesh CreateCubeWorldMesh(float width, float height, float depth, bool isDoubleTiled);
		WorldMesh CreateCenteredCubeWorldMesh(float width, float height, float depth, bool isDoubleTiled);
		WorldMesh CreatePlaneWorldMesh(float width, float height, bool isDoubleTiled);
		void      CreateWorldSolidCube(const SolidCubeInfo& cubeInfo);
		void      CreateWorldSolidPlane(const SolidPlaneInfo& planeInfo);
		void      CreatePropObject(const PropInfo& propInfo);
		void      CreateTriggerObject(const TriggerInfo& trigInfo);
		void      CreateInteractableObject(const InteractableInfo& interInfo);
		void      CreateTestEntityObject(const TestEntityInfo& testEntInfo);
		void      CreateMoveObjectEntityObject(const MoveObjectEntityInfo& movObjInfo);
		


		template<typename T>
		T& GetCompFromCtx()
		{

			T& res = registry.ctx().get<T>();
			return res;

		}


		entt::registry registry;


		std::vector<WorldMesh> worldMeshes;
		std::unordered_map<uint64_t, entt::entity> entitiesMap; // Mappa per riferimenti delle entità ECS


	private:

		EngineContext* ctx_ = nullptr;
		EngineServices engineServices_;


		bool LoadAssetFile(const char* assetFile, std::vector<std::string>& allTexturesFiles, std::vector<std::string>& allModels3DFiles, std::vector<std::string>& allAudioFiles);


		void ReadCubeDataFromFile(std::fstream& f);
		void ReadPlaneDataFromFile(std::fstream& f);
		void ReadPropDataFromFile(std::fstream& f, std::unordered_map<std::string, std::string>& modelTextureMap);
		void ReadPlayerDataFromFile(std::fstream& f);
		void ReadTriggerDataFromFile(std::fstream& f);
		void ReadInteractableDataFromFile(std::fstream& f);
		void ReadTestEntityDataFromFile(std::fstream& f);
		void ReadMoveObjectEntityDataFromFile(std::fstream& f);

	};

}