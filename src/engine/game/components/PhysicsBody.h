#pragma once
#include <box3d/id.h>
#include <entt.hpp>


namespace nothing
{

	namespace components
	{

		enum class BodyType
		{

			Static,
			Dynamic

		};


		enum class MeshType
		{

			Cube,
			Plane

		};


		struct PhysicsBody
		{

			MeshType meshType = MeshType::Cube;
			BodyType bodyType = BodyType::Static;


			float width  = 1.0f;
			float height = 1.0f;
			float depth  = 1.0f;

			
			bool isCentered = false;
			bool isTrigger  = false;


			// Lo stesso ID dell'entità a cui attacchiamo questo componenete,
			// serve per recuperarla negli User Data dal Raycast fisico
			entt::entity selfEntID;


			b3BodyId bodyID{};

		};

	}

}