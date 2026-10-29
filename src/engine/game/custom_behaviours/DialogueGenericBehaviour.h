#pragma once
#include "../basic/BaseCustomBehaviour.h"
#include "../components/UISharedValues.h"
#include <tweeny/tweeny.h>
#include <tweeny/tween.h>
#include <vector>
#include <optional>
#include <string>


namespace nothing
{

	class DialogueGenericBehaviour : public BaseCustomBehaviour
	{

	private:

		std::optional<tweeny::tween<float>> riseDialogueBoxTween;
		std::optional<tweeny::tween<float>> lowerDialogueBoxTween;
		bool isRisingDialogueBox = false;
		bool isLoweringDialogueBox = false;
		bool isInDialogue = false;
		bool displayLine = false;

	public:

		std::vector<std::string> dialogueLines;
		int currentLineIndex = 0;
		int currentCharacterIndex = 0;
		bool dialogueState = true; // VERO se il box di dialogo è SU, FALSE se GIU'
		bool canInteract = true;


		void Create()
		{

			using namespace nothing::components;


			PrintInfoMessage("All dialogue lines: " + std::to_string(dialogueLines.size()));


			riseDialogueBoxTween.emplace(tweeny::from(NOTHING_DBOX_LOWEST).to(NOTHING_DBOX_HIGHEST).during(300U).via(tweeny::easing::linear).build());
			lowerDialogueBoxTween.emplace(tweeny::from(NOTHING_DBOX_HIGHEST).to(NOTHING_DBOX_LOWEST).during(300U).via(tweeny::easing::linear).build());


			riseDialogueBoxTween->on(tweeny::event::complete, [&](auto& t) { PostMove(); return tweeny::event::response::ok; });

		}


		void Update(double deltaTime)
		{

			using namespace nothing::components;


			auto& uiSharedVals = GetContextComponent<UISharedValues>();


			if (isRisingDialogueBox)
			{

				canInteract = false;


				int32_t step = static_cast<int>(static_cast<float>(deltaTime) * 1000.0f);
				riseDialogueBoxTween->step(step);


				uiSharedVals.dialogueBoxYOffset = riseDialogueBoxTween->peek();

			}


			if (isLoweringDialogueBox)
			{

				canInteract = false;


				int32_t step = static_cast<int>(static_cast<float>(deltaTime) * 1000.0f);
				riseDialogueBoxTween->step(-step);


				uiSharedVals.dialogueBoxYOffset = riseDialogueBoxTween->peek();


				if (riseDialogueBoxTween->progress() == 0.0)
				{

					PostMove();

				}

			}


			if (displayLine)
			{

				auto& currentLine = dialogueLines[currentLineIndex];


				uiSharedVals.dialogueBoxLine += currentLine[currentCharacterIndex];
				currentCharacterIndex++;


				if (currentCharacterIndex > currentLine.size())
				{

					displayLine = false;
					currentCharacterIndex = 0;
					
				}

			}

		}


		void Interact()
		{

			if (!canInteract)
			{

				PrintInfoMessage("Wait...");
				return;

			}


			if (!isInDialogue)
			{
				
				// Fa comparire il box di dialogo
				GetContextComponent<nothing::components::UISharedValues>().dialogueBoxLine.clear();
				isRisingDialogueBox = true;

			}
			else
			{

				if (currentLineIndex >= dialogueLines.size() - 1)
				{

					// Fa scomparire il box di dialogo
					GetContextComponent<nothing::components::UISharedValues>().dialogueBoxLine.clear();
					currentLineIndex = 0;
					currentCharacterIndex = 0;
					isLoweringDialogueBox = true;
					displayLine = false;

				}
				else
				{

					GetContextComponent<nothing::components::UISharedValues>().dialogueBoxLine.clear();
					currentLineIndex++;
					currentCharacterIndex = 0;
					displayLine = true;

				}

			}

		}


		void PostMove()
		{

			switch (dialogueState)
			{

			case true:
				isRisingDialogueBox = false;
				isInDialogue = true;
				displayLine = true;
				break;


			case false:
				isLoweringDialogueBox = false;
				isInDialogue = false;
				riseDialogueBoxTween->seek(0);
				break;

			}


			canInteract = true;
			dialogueState = !dialogueState;

		}

	};

}