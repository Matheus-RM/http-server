#pragma once

#ifdef _DEBUG

	#include <iostream>

	template <class FirstT, class... Args>
	void print(const FirstT& first, const Args&... rest)
	{
		std::cout << first;

		if constexpr (sizeof...(rest) > 0)
			print(rest...);
	}

	#define MESSAGE(...) print(__VA_ARGS__);


#elif defined NDEBUG

	#define MESSAGE(...)

#else
	#error("Neither _DEBUG or NDEBUG are defined, please define one.")

#endif