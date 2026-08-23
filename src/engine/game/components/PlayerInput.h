#pragma once


namespace nothing
{
	namespace components
	{
		struct PlayerInput
		{

			float moveForward   = 0.0f;
			float moveBackwards = 0.0f;
			float moveLeft      = 0.0f;
			float moveRight     = 0.0f;
			float rotateUp      = 0.0f;
			float rotateDown    = 0.0f;
			float rotateLeft    = 0.0f;
			float rotateRight   = 0.0f;
			float mouseXDelta   = 0.0f;
			float mouseYDelta   = 0.0f;

		};
	}
}