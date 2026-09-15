#pragma once
#include <box3d/box3d.h>
#include <glm/glm.hpp>
#include <entt.hpp>
#include "../core/EngineContext.h"
#include "../game/components/PhysicsBody.h"
#include "../game/components/Transform.h"


namespace nothing
{

	// Viene passato al codice di gioco per recuperare i dati di raycast
	struct RaycastHit
	{

		glm::vec3 hitPoint;
		entt::entity hitEntityID;


		// Questo viene popolato se l'entità colpita posside EntityReference,
		// ad esempio: un InteractableArea è interagibile ma punta ad un'altra
		// entità che esegue il codice
		entt::entity targetEntity;

	};


	// SCOPO: Gestione della fisica, powered by Box3D.
	class PhysicsManager
	{
	public:

		void Init(EngineContext& ctx);
		void Update(double deltaTime);
		void InitPhysicsScene();
		void DeletePhysicsScene();
		void Shutdown();


		bool RaycastInternal(const glm::vec3& origin, const glm::vec3& direction, RaycastHit& outRay);

	private:

		b3WorldId worldID_{};
		float     timeStep_ = 1.0f / 60.0f;
		int       subSteps_ = 4;


		void ConstructCubePhysicsBody(entt::entity entID, components::Transform& tr, components::PhysicsBody& pBody);
		void ConstructPlanePhysicsBody(entt::entity entID, components::Transform& tr, components::PhysicsBody& pBody);


		// Utility
		glm::vec3 GlmVec3_FromB3(const b3Vec3& vec);
		b3Vec3    B3Vec3_FromGlm(const glm::vec3& vec);
		b3Quat    B3Quat_FromGlm(const glm::quat& quat);


		b3DebugDraw debugDraw;


		static void DebugDrawLine(b3Pos p1, b3Pos p2, b3HexColor color, void* context);


		EngineContext* ctx_ = nullptr;
		
	};

}