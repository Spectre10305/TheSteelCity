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


		struct PhysicsBody
		{

			BodyType type = BodyType::Static;


			float width  = 1.0f;
			float height = 1.0f;
			float depth  = 1.0f;


			b3BodyId bodyID{};

		};

	}

}