#pragma once


namespace nothing
{

	class BaseUIContext
	{

	public:

		virtual ~BaseUIContext() = default;

		virtual void BeginUpdate() {};
		virtual void EndUpdate() {};

	};

}