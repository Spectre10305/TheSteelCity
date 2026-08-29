#include "PhysicsManager.h"
#include "../utils/Log.h"
#include "../core/SceneManager.h"
#include "../graphics/RenderManager.h"
#include "../game/components/PhysicsBody.h"
#include "../game/components/Transform.h"
#include <glm/gtc/quaternion.hpp>


// =================================================


void nothing::PhysicsManager::Init(EngineContext& ctx)
{

	ctx_ = &ctx;
	debugDraw = b3DefaultDebugDraw();
	debugDraw.DrawSegmentFcn = DebugDrawLine;
	debugDraw.drawShapes = true;
	debugDraw.drawContactNormals = true;
	debugDraw.context = this;

}


// =================================================


void nothing::PhysicsManager::Update(double deltaTime)
{

	using namespace nothing::components;


	b3World_Step(worldID_, timeStep_, subSteps_);


	auto bodyIDsView = ctx_->sceneManager->registry.view<PhysicsBody, Transform>();


	for (auto [ent, physB, tr] : bodyIDsView.each())
	{

		b3Vec3 pos = b3Body_GetPosition(physB.bodyID);
		//b3Quat rot = b3Body_GetRotation(physB.bodyID);
		

		//glm::quat qrot = glm::quat(rot.s, glm::vec3(rot.v.x, rot.v.y, rot.v.z));


		//glm::vec3 euler = glm::eulerAngles(qrot);
		//tr.rotation.x = euler.x;
		//tr.rotation.y = euler.y;
		//tr.rotation.z = euler.z;


		tr.position.x = pos.x;
		tr.position.y = pos.y;
		tr.position.z = pos.z;
		
	}


	// Debug draw
	b3World_Draw(worldID_, &debugDraw, B3_DEFAULT_CATEGORY_BITS);
	
}


// =================================================


void nothing::PhysicsManager::InitPhysicsScene()
{

	using namespace nothing::components;


	b3WorldDef physicsWorldDef = b3DefaultWorldDef();
	physicsWorldDef.gravity = b3Vec3{ 0.0f, -1.0f, 0.0f };


	worldID_ = b3CreateWorld(&physicsWorldDef);


	auto physBodyIDView = ctx_->sceneManager->registry.view<Transform, PhysicsBody>();


	for (auto [ent, tr, pBody] : physBodyIDView.each())
	{

		b3Vec3 offset;
		offset.x = pBody.width - (pBody.width / 2);
		offset.y = pBody.height - (pBody.height / 2);
		offset.z = pBody.depth - (pBody.depth / 2);


		b3BodyDef bodyDef = b3DefaultBodyDef();
		bodyDef.position = B3Vec3_FromGlm(tr.position);
		

		switch (pBody.type)
		{

		case BodyType::Static:
			bodyDef.type = b3_staticBody;
			break;


		case BodyType::Dynamic:
			bodyDef.type = b3_dynamicBody;
			break;


		default:
			bodyDef.type = b3_staticBody;
			break;

		}


		pBody.bodyID = b3CreateBody(worldID_, &bodyDef);
		

		b3BoxHull boxHull = b3MakeOffsetBoxHull(pBody.width, pBody.height, pBody.depth, offset);
		

		b3ShapeDef shapeDef = b3DefaultShapeDef();
		shapeDef.density = 1.0f;
		shapeDef.baseMaterial.friction = 0.3f;
		shapeDef.baseMaterial.restitution = 1.0;
		
		
		b3CreateHullShape(pBody.bodyID, &shapeDef, &boxHull.base);

	}

}


// =================================================


void nothing::PhysicsManager::DeletePhysicsScene()
{

	b3DestroyWorld(worldID_);

}


// =================================================


void nothing::PhysicsManager::Shutdown()
{

	// ...

}


// =================================================


glm::vec3 nothing::PhysicsManager::GlmVec3_FromB3(const b3Vec3& vec)
{

	return glm::vec3(vec.x, vec.y, vec.z);

}


// =================================================


b3Vec3 nothing::PhysicsManager::B3Vec3_FromGlm(const glm::vec3& vec)
{

	b3Vec3 res{};
	res.x = vec.x;
	res.y = vec.y;
	res.z = vec.z;
	return res;

}


// =================================================


void nothing::PhysicsManager::DebugDrawLine(b3Pos p1, b3Pos p2, b3HexColor color, void* context)
{

	PhysicsManager* physicsManager = static_cast<PhysicsManager*>(context);


	glm::vec3 point1;
	point1.x = p1.x;
	point1.y = p1.y;
	point1.z = p1.z;


	glm::vec3 point2;
	point2.x = p2.x;
	point2.y = p2.y;
	point2.z = p2.z;


	physicsManager->ctx_->renderManager->DebugDrawLine(point1, point2);

}


// =================================================