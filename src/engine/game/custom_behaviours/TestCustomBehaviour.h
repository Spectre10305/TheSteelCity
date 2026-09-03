#pragma once
#include "../basic/BaseCustomBehaviour.h"


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

	};

}