#pragma once
#include <stdint.h>


namespace nothing
{

	namespace components
	{

		// Handler per l'ID univoco dell'entità, diverso da entt::entity.
		struct EntityID
		{

			uint64_t ID;

		};

	}

}