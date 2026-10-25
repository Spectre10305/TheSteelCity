#pragma once
#include <string>


namespace nothing
{

	// Eventi di gioco, portano degli eventi al codice dell'engine.
	// Ad esempio: cambiare mappa lo si dovrebbe fare prima o dopo
	// tutta la computazione del frame, non in mezzo. 


	struct GameEvent_ChangeMap
	{

		std::string nextMapName;

	};

}