#pragma once
#include "../basic/BaseCustomBehaviour.h"
#include "../components/Tags.h"


namespace nothing
{

	class TestCustomBehaviour : public BaseCustomBehaviour
	{

	public:

		void Create()
		{

			PrintInfoMessage("Creato");

		}


		void Interact()
		{

			PrintInfoMessage("Interagito...");

		}


		void TriggerTouch(entt::entity other)
		{

			using namespace nothing::components;


			if (sceneRegistry_->all_of<PlayerTag>(other))
			{

				PrintInfoMessage("Il giocatore e' qui...");

			}

		}

	};

}