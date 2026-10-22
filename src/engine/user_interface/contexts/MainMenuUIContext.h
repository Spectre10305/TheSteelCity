#pragma once
#include "BaseUIContext.h"

namespace nothing
{

	class MainMenuUIContext : public BaseUIContext
	{

	public:

		void BeginUpdate() override;
		void EndUpdate() override;

	};

}