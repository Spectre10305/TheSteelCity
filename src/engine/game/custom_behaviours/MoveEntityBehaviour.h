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

		entt::entity entityToTransform = entt::null;
		glm::vec3    destinationPos    = glm::vec3(0.0f, 0.0f, 0.0f);
		bool         active            = false;
		uint32_t     duration          = 1; // Secondi
		
		
		tweeny::tween<float, float, float> motionTween;


		void Create()
		{

			auto& transform = GetComponent<nothing::components::Transform>(entityToTransform);


			motionTween = tweeny::from(transform.position.x, transform.position.y, transform.position.z)
								 .to(destinationPos.x, destinationPos.y, destinationPos.x)
				                 .during(duration)
				                 .build();

		}


		void Update(double deltaTime)
		{

			if (active)
			{

				motionTween.step(static_cast<int>(static_cast<float>(deltaTime) * 1000.0f));
				motionTween.on(tweeny::event::complete, [](auto& t) {active = false; tweeny::event::response::ok});

			}

		}

	};

}