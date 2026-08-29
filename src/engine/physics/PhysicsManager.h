#pragma once
#include <box3d/box3d.h>
#include <glm/glm.hpp>
#include "../core/EngineContext.h"


namespace nothing
{

	// SCOPO: Gestione della fisica, powered by Box3D.
	class PhysicsManager
	{
	public:

		void Init(EngineContext& ctx);
		void Update(double deltaTime);
		void InitPhysicsScene();
		void DeletePhysicsScene();
		void Shutdown();

	private:

		b3WorldId worldID_{};
		float     timeStep_ = 1.0f / 60.0f;
		int       subSteps_ = 4;


		// Utility
		glm::vec3 GlmVec3_FromB3(const b3Vec3& vec);
		b3Vec3    B3Vec3_FromGlm(const glm::vec3& vec);


		b3DebugDraw debugDraw;


		static void DebugDrawLine(b3Pos p1, b3Pos p2, b3HexColor color, void* context);


		EngineContext* ctx_ = nullptr;
		
	};

}