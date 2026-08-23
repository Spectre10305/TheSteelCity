#pragma once
#include "../basic/BaseCustomBehaviour.h"
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>


namespace nothing
{

	class CameraBehaviour : public BaseCustomBehaviour
	{

	public:

		void Update(double deltaTime)
		{

			using namespace nothing::components;


			auto& cam = GetComponent<Camera>();
			auto& input = GetContextComponent<PlayerInput>();
			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);


			if (input.moveForward == 1.0f)
			{

				cam.position += cam.speed * cam.rotation;

			}


			if (input.moveBackwards == 1.0f)
			{

				cam.position -= cam.speed * cam.rotation;

			}


			if (input.moveLeft == 1.0f)
			{

				cam.position += cam.speed * -glm::normalize(glm::cross(cam.rotation, up));

			}


			if (input.moveRight == 1.0f)
			{

				cam.position += cam.speed * glm::normalize(glm::cross(cam.rotation, up));

			}


			if (input.rotateUp == 1.0f)
			{

				cam.rotation = glm::rotate(cam.rotation, glm::radians(cam.rotSens), glm::normalize(glm::cross(cam.rotation, up)));

			}


			if (input.rotateDown == 1.0f)
			{

				cam.rotation = glm::rotate(cam.rotation, glm::radians(-cam.rotSens), glm::normalize(glm::cross(cam.rotation, up)));

			}


			if (input.rotateLeft == 1.0f)
			{

				cam.rotation = glm::rotate(cam.rotation, glm::radians(cam.rotSens), up);

			}


			if (input.rotateRight == 1.0f)
			{

				cam.rotation = glm::rotate(cam.rotation, glm::radians(-cam.rotSens), up);

			}

		}

	};

}