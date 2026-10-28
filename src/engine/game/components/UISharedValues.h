#pragma once
#include <string>
#define NOTHING_DBOX_LOWEST 630.0f
#define NOTHING_DBOX_HIGHEST 280.0f


namespace nothing
{

	namespace components
	{

		struct UISharedValues
		{

			int   fadeScreenAlpha = 255;
			float healthBarValue  = 0.0f;
			float dialogueBoxYOffset = NOTHING_DBOX_LOWEST;
			std::string dialogueBoxLine = "";

		};

	}

}