#pragma once
#include <glm/glm.hpp>


namespace nothing
{

	namespace components
	{

		// Contenitore globale di valori modificabili dal sistema UI
		struct UIDebugValues
		{

			float playerPosX      = 0.0f;
			float playerPosY      = 0.0f;
			float playerPosZ      = 0.0f;
			float playerWalkSpeed = 0.0f;
			float playerRunSpeed  = 0.0f;
			int   playerHealth    = 0;


			// RenderManager
			float gExposure       = 1.0f;
			float gLightPosX      = -2.0f;
			float gLightPosY      = 4.0f;
			float gLightPosZ      = -1.0f;
			float gDepthNear      = 0.1f;
			float gDepthFar       = 7.5f;
			float gDepthDimension = 10.0f;

		};

	}

}