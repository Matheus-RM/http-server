#pragma once

#include <string>
#include <functional>

#define BIND_FILTER(FILTER) std::bind(FILTER, std::placeholders::_1)
#define BIND_CLASS_FILTER(FILTER) std::bind(&FILTER, this, std::placeholders::_1)

namespace Filters
{
	bool checkText(const std::string& text);
	bool checkInteger(const std::string& text);
	bool checkUnsigned(const std::string& text);
}