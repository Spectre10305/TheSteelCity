#pragma once


#define NOTHING_WINSTYLE_TECH 0
#define NOTHING_WINSTYLE_INDUSTRIAL 1
#define NOTHING_WINSTYLE_EVIL 2


namespace nothing
{

	// SCOPO: Wrapper per le funzioni IMGUI per fare Push() / Pop() dei vari stili
	class UIStyleHelper
	{

	public:

		static void AutoPushStyle(int style);
		static void PopStyle();

	private:

		static void PushTech();
		static void PushIndustrial();
		static void PushEvil();

	};

}