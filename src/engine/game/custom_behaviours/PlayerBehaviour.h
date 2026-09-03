#pragma once
#include "../basic/BaseCustomBehaviour.h"
#include "../components/Camera.h"
#include "../components/Transform.h"
#include "../components/Velocity.h"
#include "../components/PlayerInput.h"
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>


namespace nothing
{

	class PlayerBehaviour : public BaseCustomBehaviour
	{

	public:

		glm::vec3 cameraOffset = glm::vec3(-3.0f, 6.0f, 3.0f);
		glm::vec3 cameraTopOffset = glm::vec3(0.001f, 6.0f, 0.001f); // Vista "dritto per dritto" dall'alto. Test
		float speed = 0.0f;


		void Create()
		{

			using namespace nothing::components;


			auto& transform = GetComponent<Transform>();
			auto& camera = GetComponent<Camera>();


			// Posiziona la telecamera in alto e direziona la visione verso il giocatore
			// con un piccolo offset in Y di -0.5 (in realtà dovrebbe essere +0.5 ma non so perchè...)
			camera.position = transform.position + glm::vec3(0.0f, -0.5f, 0.0f) + cameraOffset;


			glm::vec3 dir = glm::normalize(transform.position - camera.position);
			camera.rotation = dir;
			//camera.rotation = glm::vec3(0.408248f, -0.816497f, -0.408248f);

		}


		void Update(double deltaTime)
		{

			using namespace nothing::components;


			auto& transform = GetComponent<Transform>();
			auto& cam       = GetComponent<Camera>();
			auto& input     = GetContextComponent<PlayerInput>();
			auto& velocity  = GetComponent<Velocity>();


			glm::quat movYaw = glm::angleAxis(glm::radians(-input.mouseXDelta), glm::vec3(0, 1, 0));


			transform.rotation = movYaw * transform.rotation;


			/*
			glm::vec3 forward;
			float yaw = glm::radians(transform.rotation.y);
			forward.x = sin(yaw);
			forward.y = 0.0f;
			forward.z = cos(yaw);
			*/

			glm::vec3 forward = transform.rotation * glm::vec3(0.0f, 0.0f, 1.0f);


			speed = input.running ? 2.0f : 1.5f;


			if (input.moveForward == 1.0f)
			{

				//transform.position += forward * speed * static_cast<float>(deltaTime);


				// Impostiamo solo la velocity, il movimento effettivo avverrà nel PhysicsManager
				velocity.value = forward * speed * static_cast<float>(deltaTime);

			}
			else
			{

				velocity.value = glm::vec3(0.0f, 0.0f, 0.0f);

			}

		}


		void LateUpdate(double deltaTime)
		{

			using namespace nothing::components;


			auto& transform = GetComponent<Transform>();
			auto& cam = GetComponent<Camera>();


			glm::vec3 desiredPos = transform.position + cameraOffset;
			cam.position = glm::mix(cam.position, desiredPos, 5.0 * static_cast<float>(deltaTime));

		}

	};

}