#pragma once
#include <string>
#include "../basic/BaseCustomBehaviour.h"
#include "../basic/GameEvents.h"
#include "../components/Tags.h"


namespace nothing
{

	class MapChangeBehaviour : public BaseCustomBehaviour
	{

	public:

		std::string nextMapName;


		void TriggerTouch(entt::entity visitor)
		{

			using namespace nothing::components;


			if (sceneRegistry_->all_of<PlayerTag>(visitor))
			{

				sceneEventDispatcher_->enqueue<nothing::GameEvent_ChangeMap>(nextMapName);

			}

		}

	};

}