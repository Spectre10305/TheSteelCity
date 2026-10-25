#pragma once
#include "../basic/BaseCustomBehaviour.h"
#include <vector>
#include <string>


namespace nothing
{

	class DialogueGenericBehaviour : public BaseCustomBehaviour
	{

	public:

		std::vector<std::string> dialogueLines;
		int currentLineIndex = 0;


		void Interact()
		{

			if (currentLineIndex < dialogueLines.size())
			{

				PrintInfoMessage(dialogueLines.at(currentLineIndex));
				currentLineIndex++;

			}
			else
			{

				PrintInfoMessage("Finished dialogue");

			}

		}

	};

}