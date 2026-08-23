#pragma once
#include <glm\glm.hpp>


namespace nothing
{

	namespace components
	{

		struct Camera
		{

			glm::vec3   position         = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3   rotation         = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::mat4   viewMatrix       = glm::mat4(1.0f);
			glm::mat4   projectionMatrix = glm::mat4(1.0f);
			float       speed            = 0.05f;
			float       rotSens          = 1.0f;
			float       fov              = 60.0f;

		};

	}

}