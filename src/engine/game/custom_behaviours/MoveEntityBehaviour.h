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

		uint64_t entityToTransformID   = 0;
		glm::vec3    destinationPos    = glm::vec3(0.0f, 0.0f, 0.0f);
		bool         active            = false;
		bool         completed         = false;
		uint32_t     duration          = 1000; // millisecondi
		
		
		// Con std::optional non abbiamo il problema dell'avere un tweeny::tween senza costruttore
		std::optional<tweeny::tween<float, float, float>> motionTween;
		entt::entity entityToTransform = entt::null;


		void Create()
		{

			entityToTransform = ResolveEntityID(entityToTransformID);


			if (sceneRegistry_->valid(entityToTransform))
			{

				PrintInfoMessage("Valid transform entity to move");

			}


			auto& transform = GetComponent<nothing::components::Transform>(entityToTransform);




			motionTween.emplace(tweeny::from(transform.position.x, transform.position.y, transform.position.z)
										.to(destinationPos.x, destinationPos.y, destinationPos.z)
										.during(duration)
										.via(tweeny::easing::backInOut)
										.build());


			motionTween->on(tweeny::event::complete, [&](auto& t) {AfterMove(); return tweeny::event::response::ok; });

		}


		void Update(double deltaTime)
		{

			if (completed)
				return;


			if (active)
			{

				auto& transform = GetComponent<nothing::components::Transform>(entityToTransform);


				motionTween->step(static_cast<int>(static_cast<float>(deltaTime) * 1000.0f));


				auto move = motionTween->peek();


				transform.position.x = move[0];
				transform.position.y = move[1];
				transform.position.z = move[2];

			}

		}


		void Interact()
		{

			PrintInfoMessage("Interact, moving object...");
			active = true;

		}


		void TriggerTouch(entt::entity other)
		{

			using namespace nothing::components;


			if (sceneRegistry_->all_of<PlayerTag>(other))
			{

				PrintInfoMessage("TriggerTouch, moving object...");
				active = true;

			}

		}

	private:

		void AfterMove()
		{

			active = false;
			completed = true;

			
			PrintInfoMessage("Final Pos: " + std::to_string(destinationPos.x) + " - " + std::to_string(destinationPos.y) + " - " + std::to_string(destinationPos.z));

		}

	};

}