#include "PhysicsManager.h"
#include "../utils/Log.h"
#include "../core/SceneManager.h"
#include "../graphics/RenderManager.h"
#include "../core/InputManager.h"
#include "../game/components/Velocity.h"
#include "../game/components/Tags.h"
#include "../game/components/NameTag.h"
#include "../game/components/EntityReference.h"
#include "../game/components/CustomBehaviour.h"
#include <glm/gtc/quaternion.hpp>


// =================================================


void nothing::PhysicsManager::Init(EngineContext& ctx)
{

	ctx_ = &ctx;


	debugDraw = b3DefaultDebugDraw();
	debugDraw.DrawSegmentFcn     = DebugDrawLine;
	debugDraw.drawShapes         = true;
	debugDraw.drawContactNormals = true;
	debugDraw.context            = this;

}


// =================================================


void nothing::PhysicsManager::Update(double deltaTime)
{

	using namespace nothing::components;


	if (!ctx_->isGamePaused)
	{

		// Applica la fisica del giocatore
		auto playerView = ctx_->sceneManager->registry.view<Transform, PhysicsBody, Velocity, PlayerTag>();


		for (auto [ent, tr, pBody, vel] : playerView.each())
		{

			b3Body_SetLinearVelocity(pBody.bodyID, B3Vec3_FromGlm(vel.value * glm::vec3(100)));


			b3Vec3 pos = b3Body_GetPosition(pBody.bodyID);
			

			tr.position = GlmVec3_FromB3(pos);

		}


		// Applica la fisica degli oggetti fisici
		b3World_Step(worldID_, timeStep_, subSteps_);


		b3SensorEvents sensorsEvents = b3World_GetSensorEvents(worldID_);


		for (int i = 0; i < sensorsEvents.beginCount; ++i)
		{

			nothing::LogInfo("We got trigger collision....");


			b3SensorBeginTouchEvent* begEvent = sensorsEvents.beginEvents + i;


			entt::entity triggerEnt = static_cast<entt::entity>(reinterpret_cast<std::uintptr_t>(b3Shape_GetUserData(begEvent->sensorShapeId)));
			entt::entity visitorEnt = static_cast<entt::entity>(reinterpret_cast<std::uintptr_t>(b3Shape_GetUserData(begEvent->visitorShapeId)));


			if (!ctx_->sceneManager->registry.valid(triggerEnt) || !ctx_->sceneManager->registry.valid(visitorEnt))
			{

				nothing::LogError("Something went wrong when getting trigger collision data");
				continue;

			}


			auto entRef = ctx_->sceneManager->registry.try_get<EntityReference>(triggerEnt);


			if (entRef != nullptr)
			{

				auto customBeh = ctx_->sceneManager->registry.try_get<CustomBehaviour>(entRef->other);


				if (customBeh != nullptr)
				{

					customBeh->customBehaviour->TriggerTouch(visitorEnt);

				}

			}

		}


		auto bodyIDsView = ctx_->sceneManager->registry.view<PhysicsBody, Transform>();


		for (auto [ent, physB, tr] : bodyIDsView.each())
		{

			// Salta gli oggetti statici
			if (physB.bodyType == BodyType::Static)
				continue;


			b3Vec3 pos = b3Body_GetPosition(physB.bodyID);
			b3Quat rot = b3Body_GetRotation(physB.bodyID);


			glm::quat qrot = glm::quat(rot.s, rot.v.x, rot.v.y, rot.v.z);


			//glm::vec3 euler = glm::eulerAngles(qrot);
			//tr.rotation.x = euler.x;
			//tr.rotation.y = euler.y;
			//tr.rotation.z = euler.z;


			tr.rotation = qrot;


			tr.position.x = pos.x;
			tr.position.y = pos.y;
			tr.position.z = pos.z;

		}


		auto playerView2 = ctx_->sceneManager->registry.view<Transform, PhysicsBody, PlayerTag>();


		for (auto [ent, tr, pBody] : playerView2.each())
		{

			// Raycast di interazione
			if (ctx_->inputManager->IsActionTriggered(GameAction::Use))
			{

				nothing::LogInfoVector("Player position: ", tr.position.x, tr.position.y, tr.position.z);


				// Trova il forward, la direzione in cui il personaggio guarda
				glm::vec3 forward = glm::normalize(tr.rotation * glm::vec3(0.0f, 0.0f, 1.0f));


				nothing::LogInfoVector("Forward vector: ", forward.x, forward.y, forward.z);


				float       rayLenght   = 1.0f;
				b3Vec3      rayOrigin   = B3Vec3_FromGlm(tr.position + forward * 0.1f);
				rayOrigin.y             += 0.5f;
				glm::vec3   translation = forward * rayLenght;
				b3RayResult res         = b3World_CastRayClosest(worldID_, rayOrigin, B3Vec3_FromGlm(translation), b3DefaultQueryFilter());


				if (res.hit)
				{

					nothing::LogInfo("Hit something");
					

					void* userData = b3Shape_GetUserData(res.shapeId);


					entt::entity ent = static_cast<entt::entity>(reinterpret_cast<std::uintptr_t>(userData));


					if (ent != entt::null)
					{

						nothing::LogInfo("We got entity!");


						if (ctx_->sceneManager->registry.all_of<nothing::components::CustomBehaviour>(ent))
						{

							nothing::LogInfo("Entity got CustomBehaviour component");


							auto beh = ctx_->sceneManager->registry.try_get<nothing::components::CustomBehaviour>(ent);


							if (beh != nullptr)
							{

								beh->customBehaviour->Interact();

							}

						}

					}

				}


				// Solo per debug
				b3Vec3 rayEnd = rayOrigin + B3Vec3_FromGlm(translation);
				ctx_->renderManager->DebugDrawLine(GlmVec3_FromB3(rayOrigin), GlmVec3_FromB3(rayEnd));

			}

		}


		// Debug draw
		b3World_Draw(worldID_, &debugDraw, B3_DEFAULT_CATEGORY_BITS);

	}

}


// =================================================


void nothing::PhysicsManager::InitPhysicsScene()
{

	using namespace nothing::components;


	b3WorldDef physicsWorldDef = b3DefaultWorldDef();
	physicsWorldDef.gravity = b3Vec3{ 0.0f, -1.0f, 0.0f };


	worldID_ = b3CreateWorld(&physicsWorldDef);


	auto physBodyIDView = ctx_->sceneManager->registry.view<Transform, PhysicsBody>(entt::exclude<PlayerTag>);


	for (auto [ent, tr, pBody] : physBodyIDView.each())
	{

		switch (pBody.meshType)
		{

		case MeshType::Cube:
			ConstructCubePhysicsBody(tr, pBody);
			break;


		case MeshType::Plane:
			ConstructPlanePhysicsBody(tr, pBody);
			break;


		default:
			nothing::LogWarning("Unsupported mesh type evaluated while constructing physics bodies, skipping...");
			break;
		}

	}
	

	// Inizializza le collisioni del Player
	auto playerView = ctx_->sceneManager->registry.view<Transform, PhysicsBody, PlayerTag>();


	for (auto [ent, tr, pBody] : playerView.each())
	{

		b3BodyDef playerBodyDef = b3DefaultBodyDef();
		playerBodyDef.position = B3Vec3_FromGlm(tr.position);
		playerBodyDef.gravityScale = 0.0f;
		playerBodyDef.type = b3_dynamicBody;


		b3MotionLocks playerMotionLocks{};
		playerMotionLocks.angularX = true;
		playerMotionLocks.angularZ = true;
		playerBodyDef.motionLocks = playerMotionLocks;
		

		pBody.bodyID = b3CreateBody(worldID_, &playerBodyDef);


		b3Capsule playerCapsule{};
		playerCapsule.center1 = b3Vec3{ 0.0f, 0.2f, 0.0f };
		playerCapsule.center2 = b3Vec3{ 0.0f, 0.9f, 0.0f };
		playerCapsule.radius = 0.2f;


		b3ShapeDef playerShapeDef = b3DefaultShapeDef();
		playerShapeDef.baseMaterial.restitution = 0.0f;
		playerShapeDef.baseMaterial.friction = 0.1f;
		playerShapeDef.enableSensorEvents = true;
		playerShapeDef.userData = reinterpret_cast<void*>(static_cast<std::uintptr_t>(pBody.selfEntID));


		b3CreateCapsuleShape(pBody.bodyID, &playerShapeDef, &playerCapsule);

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


void nothing::PhysicsManager::ConstructCubePhysicsBody(components::Transform& tr, components::PhysicsBody& pBody)
{

	using namespace nothing::components;

	
	b3BodyDef bodyDef = b3DefaultBodyDef();
	bodyDef.position = B3Vec3_FromGlm(tr.position);


	switch (pBody.bodyType)
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


	b3Vec3 offset;
	offset.x = pBody.width - (pBody.width / 2.0f);
	offset.y = pBody.height - (pBody.height / 2.0f);
	offset.z = pBody.depth - (pBody.depth / 2.0f);


	b3BoxHull boxHull{};


	if (pBody.isCentered)
	{

		boxHull = b3MakeBoxHull(pBody.width / 2.0f, pBody.height / 2.0f, pBody.depth / 2.0f);

	}
	else
	{

		boxHull = b3MakeOffsetBoxHull(offset.x, offset.y, offset.z, offset);

	}


	//b3BoxHull boxHull = b3MakeOffsetBoxHull(pBody.width, pBody.height, pBody.depth, offset);


	b3ShapeDef shapeDef = b3DefaultShapeDef();
	shapeDef.density = 1.0f;
	shapeDef.baseMaterial.friction = 0.3f;
	shapeDef.baseMaterial.restitution = 0.5f;
	shapeDef.userData = reinterpret_cast<void*>(static_cast<std::uintptr_t>(pBody.selfEntID));


	if (pBody.isTrigger)
	{

		shapeDef.isSensor = true;
		shapeDef.enableSensorEvents = true;

	}


	b3CreateHullShape(pBody.bodyID, &shapeDef, &boxHull.base);

}


// =================================================


void nothing::PhysicsManager::ConstructPlanePhysicsBody(components::Transform & tr, components::PhysicsBody& pBody)
{

	using namespace nothing::components;


	b3BodyDef bodyDef = b3DefaultBodyDef();
	bodyDef.position = B3Vec3_FromGlm(tr.position);


	b3Quat rot;
	rot.s = tr.rotation.w;
	rot.v.x = tr.rotation.x;
	rot.v.y = tr.rotation.y;
	rot.v.z = tr.rotation.z;
	bodyDef.rotation = rot;


	bodyDef.type = b3_staticBody;


	// Questo non dovrebbe servire per ora, dato che il bodyType è sempre
	// forzato a b3_staticBody
	/*
	switch (pBody.bodyType)
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
	*/


	pBody.bodyID = b3CreateBody(worldID_, &bodyDef);


	b3Vec3 offset;
	offset.x = pBody.width - (pBody.width / 2.0f);
	offset.y = pBody.height - (pBody.height / 2.0f);
	offset.z = pBody.depth - (pBody.depth / 2.0f);
	

	b3BoxHull boxHull{};


	if (pBody.isCentered)
	{

		boxHull = b3MakeBoxHull(pBody.width / 2.0f, pBody.height / 2.0f, pBody.depth / 2.0f);

	}
	else
	{

		boxHull = b3MakeOffsetBoxHull(offset.x, offset.y, offset.z, offset);

	}


	//b3BoxHull boxHull = b3MakeOffsetBoxHull(pBody.width, pBody.height, pBody.depth, offset);


	b3ShapeDef shapeDef = b3DefaultShapeDef();
	shapeDef.density = 1.0f;
	shapeDef.baseMaterial.friction = 0.3f;
	shapeDef.baseMaterial.restitution = 0.5f;
	shapeDef.userData = reinterpret_cast<void*>(static_cast<std::uintptr_t>(pBody.selfEntID));


	b3CreateHullShape(pBody.bodyID, &shapeDef, &boxHull.base);

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