#pragma once


namespace nothing
{

	// Per mappare dei valori tra un minimo e un massimo. Grazie Arduino!
	template<typename T>
	inline T Map(T val, T inMin, T inMax, T outMin, T outMax)
	{

		return(val - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;

	}

}