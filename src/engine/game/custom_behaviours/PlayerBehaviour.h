#pragma once
#include "../basic/BaseCustomBehaviour.h"
#include "../components/Camera.h"
#include "../components/Transform.h"
#include "../components/Velocity.h"
#include "../components/PlayerInput.h"
#include "../components/UIDebugValues.h"
#include "../components/Tags.h"
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>


namespace nothing
{

	class PlayerBehaviour : public BaseCustomBehaviour
	{

	public:

		glm::vec3 cameraOffset = glm::vec3(-3.0f, 6.0f, 3.0f);
		glm::vec3 cameraTopOffset = glm::vec3(0.001f, 6.0f, 0.001f); // Vista "dritto per dritto" dall'alto. Test.
		float speed = 0.0f;


		float walkSpeed = 1.5f;
		float runSpeed = 2.0f;


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


			auto& UIDgbVals = GetContextComponent<UIDebugValues>();
			UIDgbVals.playerWalkSpeed = walkSpeed;
			UIDgbVals.playerRunSpeed = runSpeed;

		}


		void Update(double deltaTime)
		{

			using namespace nothing::components;


			auto& transform = GetComponent<Transform>();
			auto& cam = GetComponent<Camera>();
			auto& input = GetContextComponent<PlayerInput>();
			auto& UIDgbVals = GetContextComponent<UIDebugValues>();
			auto& velocity = GetComponent<Velocity>();


			glm::quat movYaw = glm::angleAxis(glm::radians(-input.mouseXDelta), glm::vec3(0, 1, 0));
			transform.rotation = movYaw * transform.rotation;
			glm::vec3 forward = transform.rotation * glm::vec3(0.0f, 0.0f, 1.0f);


			speed = input.running ? UIDgbVals.playerRunSpeed : UIDgbVals.playerWalkSpeed;


			if (input.moveForward == 1.0f)
			{

				//transform.position += forward * speed * static_cast<float>(deltaTime);


				// Impostiamo solo la velocity, il movimento effettivo avverrà nel PhysicsManager
				velocity.value = forward * speed * static_cast<float>(deltaTime);
				velocity.value.y += -1.0f * deltaTime;
				

			}
			else
			{

				velocity.value = glm::vec3(0.0f, 0.0f, 0.0f);

			}


			if (input.useKeyPressed)
			{

				RaycastHit hit;


				glm::vec3 rayOrigin = transform.position + forward * 0.1f;
				rayOrigin.y += 0.5f;


				if (Raycast(rayOrigin, forward, hit))
				{

					if (sceneRegistry_->valid(hit.hitEntityID) && sceneRegistry_->valid(hit.targetEntity) && sceneRegistry_->all_of<InteractableTag>(hit.hitEntityID))
					{

						auto beh = sceneRegistry_->try_get<CustomBehaviour>(hit.targetEntity);


						if (beh)
						{

							beh->customBehaviour->Interact();

						}

					}

				}

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