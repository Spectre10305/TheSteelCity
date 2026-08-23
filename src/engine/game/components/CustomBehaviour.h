#pragma once
#include <memory>
#include "../basic/BaseCustomBehaviour.h"


namespace nothing
{

	namespace components
	{

		struct CustomBehaviour
		{

			std::unique_ptr<nothing::BaseCustomBehaviour> customBehaviour;

		};

	}

}