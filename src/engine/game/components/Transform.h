#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>


namespace nothing
{

	namespace components
	{

		struct Transform
		{

			glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

		};

	}

}