#pragma once
#include "../basic/BaseCustomBehaviour.h"
#include "../components/Camera.h"
#include "../components/Transform.h"
#include "../components/PlayerInput.h"
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <format>


namespace nothing
{

	class PlayerBehaviour : public BaseCustomBehaviour
	{

	public:

		glm::vec3 cameraOffset = glm::vec3(-3.0f, 6.0f, 3.0f);
		float speed = 0.0f;


		void Create()
		{

			using namespace nothing::components;


			auto& transform = GetComponent<Transform>();
			auto& camera = GetComponent<Camera>();


			// Posiziona le telecamera in alto e direziona la visione verso il giocatore
			glm::vec3 posPlusYOffset = transform.position + glm::vec3(0.0f, 0.5f, 0.0f);
			glm::vec3 dir = glm::normalize(posPlusYOffset - cameraOffset);
			camera.rotation = dir;

		}


		void Update(double deltaTime)
		{

			using namespace nothing::components;


			auto& transform = GetComponent<Transform>();
			auto& cam = GetComponent<Camera>();
			auto& input = GetContextComponent<PlayerInput>();
			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);


			transform.rotation.y -= input.mouseXDelta * 100.0f * deltaTime;


			glm::vec3 forward;
			float yaw = glm::radians(transform.rotation.y);
			forward.x = sin(yaw);
			forward.y = 0.0f;
			forward.z = cos(yaw);
			

			speed = input.running ? 2.0f : 1.5f;


			if (input.moveForward == 1.0f)
			{

				transform.position += forward * speed * static_cast<float>(deltaTime);

			}

		}


		void LateUpdate(double deltaTime)
		{

			using namespace nothing::components;


			auto& transform = GetComponent<Transform>();
			auto& cam = GetComponent<Camera>();


			glm::vec3 desiredPos = transform.position + cameraOffset;
			cam.position = glm::mix(cam.position, desiredPos, 5.0 * deltaTime);

		}

	};

}