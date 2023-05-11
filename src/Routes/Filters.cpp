#include "Routes/Filters.hpp"

bool Filters::checkText(const std::string& text)
{
	return true;
}

bool Filters::checkInteger(const std::string& text)
{
	if(text.empty())
		return false;

	if(!isdigit(text[0]) && text[0] != '-' && text[0] != '+')
		return false;
	
	return (text.substr(1).find_first_not_of("0123456789") == std::string::npos);
}

bool Filters::checkUnsigned(const std::string& text)
{
	if(text.empty())
		return false;

	if(!isdigit(text[0]) && text[0] != '+')
		return false;
	
	return (text.substr(1).find_first_not_of("0123456789") == std::string::npos);
}
