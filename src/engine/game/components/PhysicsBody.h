#pragma once
#include <box3d/id.h>


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


			b3BodyId bodyID{};

		};

	}

}