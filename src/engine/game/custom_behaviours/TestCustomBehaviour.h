#pragma once
#include "../basic/BaseCustomBehaviour.h"
#include "../components/Tags.h"
#include "../components/TestingComponent.h"


namespace nothing
{

	class TestCustomBehaviour : public BaseCustomBehaviour
	{

	public:

		void Create()
		{

			PrintInfoMessage("Creato");

		}


		void TriggerTouch(entt::entity other)
		{

			using namespace nothing::components;


			auto testComp = GetComponent<TestingComponent>();


			if (sceneRegistry_->all_of<PlayerTag>(other))
			{

				PrintInfoMessage("Il giocatore e' qui...");
				PrintInfoMessage(std::to_string(testComp.garbageValue1));
				PrintInfoMessage(std::to_string(testComp.garbageValue2));
				PrintInfoMessage(std::to_string(testComp.garbageValue3));

			}

		}

	};

}