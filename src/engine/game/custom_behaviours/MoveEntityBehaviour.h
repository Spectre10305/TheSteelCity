#pragma once
#include "../basic/BaseCustomBehaviour.h"
#include "../components/Transform.h"
#include <tweeny/tweeny.h>
#include <tweeny/tween.h>


namespace nothing
{

	class MoveEntityBehaviour : public BaseCustomBehaviour
	{

	public:

		uint64_t     entityToTransformID = 0;
		glm::vec3    destinationPos      = glm::vec3(0.0f, 0.0f, 0.0f);
		bool         twoWays             = false;
		bool         ignoreY             = false;
		uint32_t     duration            = 1000; // millisecondi
		
		
		// Con std::optional non abbiamo il problema dell'avere un tweeny::tween senza costruttore, altrimenti il compilatore si lamenta...
		std::optional<tweeny::tween<float, float, float>> forwardMotionTween;
		std::optional<tweeny::tween<float, float, float>> backwardMotionTween;


		entt::entity entityToTransform = entt::null;


		void Create()
		{

			entityToTransform = ResolveEntityID(entityToTransformID);


			if (sceneRegistry_->valid(entityToTransform))
			{

				PrintInfoMessage("Valid transform entity to move");


				auto& transform = GetComponent<nothing::components::Transform>(entityToTransform);


				prevPos = transform.position;


				forwardMotionTween.emplace(tweeny::from(transform.position.x, transform.position.y, transform.position.z)
					.to(destinationPos.x, ignoreY ? transform.position.y : destinationPos.y, destinationPos.z)
					.during(duration)
					.via(tweeny::easing::backInOut)
					.build());


				backwardMotionTween.emplace(tweeny::from(destinationPos.x, ignoreY ? transform.position.y : destinationPos.y, destinationPos.z)
					.to(prevPos.x, ignoreY ? transform.position.y : prevPos.y, prevPos.z)
					.during(duration)
					.via(tweeny::easing::backInOut)
					.build());


				forwardMotionTween->on(tweeny::event::complete, [&](auto& t) {AfterMove(); return tweeny::event::response::ok; });
				backwardMotionTween->on(tweeny::event::complete, [&](auto& t) {AfterMove(); return tweeny::event::response::ok; });

			}
			else
			{

				PrintWarningMessage("INVALID transform entity to move");

			}

		}


		void Update(double deltaTime)
		{

			if (completed)
				return;


			if (active)
			{

				int32_t step = static_cast<int>(static_cast<float>(deltaTime) * 1000.0f);


				auto& transform = GetComponent<nothing::components::Transform>(entityToTransform);


				if (moveDir)
				{

					forwardMotionTween->step(step);


					auto forwardPos = forwardMotionTween->peek();
					transform.position.x = forwardPos[0];
					transform.position.y = forwardPos[1];
					transform.position.z = forwardPos[2];

				}
				else
				{

					backwardMotionTween->step(step);


					auto backwardPos = backwardMotionTween->peek();
					transform.position.x = backwardPos[0];
					transform.position.y = backwardPos[1];
					transform.position.z = backwardPos[2];

				}

			}

		}


		void Interact()
		{

			active = true;

		}


		void TriggerTouch(entt::entity other)
		{

			using namespace nothing::components;


			if (sceneRegistry_->all_of<PlayerTag>(other))
			{

				active = true;

			}

		}

	private:

		bool      moveDir   = true; // VERO, avanti. FALSO, indietro
		bool      active    = false;
		bool      completed = false;
		glm::vec3 prevPos   = glm::vec3(0.0f, 0.0f, 0.0f);


		void AfterMove()
		{

			PrintInfoMessage("Final Pos: " + std::to_string(destinationPos.x) + " - " + std::to_string(destinationPos.y) + " - " + std::to_string(destinationPos.z));


			active = false;


			if (twoWays)
			{

				// Se la direzione è AVANTI imposta il frame del tween inverso a 0.
				// Se la direzione è INDIETRO, imposta il frame del tween normale a 0.
				// Questo evita che la posizione dell'oggetto da muovere "salti" dalla
				// fine all'inizio e viceversa
				if (moveDir)
				{

					backwardMotionTween->seek(0);

				}
				else
				{

					forwardMotionTween->seek(0);

				}


				moveDir = !moveDir;
				PrintInfoMessage("Changed direction");

			}
			else
			{

				completed = true;

			}

		}

	};

}