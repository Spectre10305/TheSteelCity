#pragma once
#include <glm/glm.hpp>


namespace nothing
{

	namespace components
	{

		struct Transform
		{

			glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);

		};

	}

}