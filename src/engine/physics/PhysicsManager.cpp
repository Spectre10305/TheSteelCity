#include "PhysicsManager.h"
#include "../utils/Log.h"


// =================================================


void nothing::PhysicsManager::Init()
{

	b3WorldDef physicsWorldDef = b3DefaultWorldDef();
	physicsWorldDef.gravity = b3Vec3{ 0.0f, -10.0f, 0.0f };
	

	worldID_ = b3CreateWorld(&physicsWorldDef);


	// Esempio tattico nucleare:


	// Terreno
	b3BodyDef exmapleGroundBodyDef = b3DefaultBodyDef();
	exmapleGroundBodyDef.position = b3Vec3{ 0.0f, 0.0f, 0.0f };


	b3BodyId   exampleGroundID       = b3CreateBody(worldID_, &exmapleGroundBodyDef);
	b3BoxHull  exampleGroundBoxHull  = b3MakeBoxHull(5.0f, 1.0f, 5.0f);
	b3ShapeDef exampleGroundShapeDef = b3DefaultShapeDef();
	

	b3CreateHullShape(exampleGroundID, &exampleGroundShapeDef, &exampleGroundBoxHull.base);


	// Cubo, viene mosso dalla gravità
	b3BodyDef bodyDef = b3DefaultBodyDef();
	bodyDef.type = b3_dynamicBody;
	bodyDef.position = b3Vec3{ 0.0f, 4.0f, 0.0f };


	           bodyId     = b3CreateBody(worldID_, &bodyDef);
	b3BoxHull  dynamicBox = b3MakeCubeHull(1.0f);
	b3ShapeDef shapeDef   = b3DefaultShapeDef();


	shapeDef.density = 1.0f;
	shapeDef.baseMaterial.friction = 0.3f;


	b3CreateHullShape(bodyId, &shapeDef, &dynamicBox.base);
	
}


// =================================================


void nothing::PhysicsManager::Update(double deltaTime)
{

	b3World_Step(worldID_, timeStep_, subSteps_);

	b3Vec3 pos = b3Body_GetPosition(bodyId);
	b3Quat rot = b3Body_GetRotation(bodyId);

	printf("%4.2f %4.2f %4.2f %4.2f %4.2f %4.2f %4.2f\n",
		pos.x, pos.y, pos.z,
		rot.v.x, rot.v.y, rot.v.z, rot.s);
	
}


// =================================================


void nothing::PhysicsManager::Shutdown()
{

	b3DestroyWorld(worldID_);

}


// =================================================