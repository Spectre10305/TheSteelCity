#pragma once
#include <string>
#include "../basic/BaseCustomBehaviour.h"
#include "../basic/GameEvents.h"
#include "../components/Tags.h"
#include "../components/UISharedValues.h"


namespace nothing
{

	class MapChangeBehaviour : public BaseCustomBehaviour
	{

	public:


		std::string nextMapName;


		void Update(double deltaTime)
		{

			using namespace nothing::components;


			if (doPanelFadeIn)
			{

				auto& uiSharedVals = GetContextComponent<UISharedValues>();


				uiSharedVals.fadeScreenAlpha += 3;


				if (uiSharedVals.fadeScreenAlpha > 255)
				{

					uiSharedVals.fadeScreenAlpha = 255;
					sceneEventDispatcher_->enqueue<nothing::GameEvent_ChangeMap>(nextMapName);
					doPanelFadeIn = false;

				}

			}

		}


		void TriggerTouch(entt::entity visitor)
		{

			using namespace nothing::components;


			if (sceneRegistry_->all_of<PlayerTag>(visitor) && !inTransition)
			{

				doPanelFadeIn = true;
				inTransition = true;

			}
			else
			{

				PrintInfoMessage("Already in transition.");

			}

		}

	private:

		bool doPanelFadeIn = false;
		bool inTransition = false;

	};

}