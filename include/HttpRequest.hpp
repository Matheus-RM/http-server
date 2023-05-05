#pragma once

#include <vector>
#include <string>

struct Argument
{
	std::string value;
	std::string type;
};

class HttpRequest
{
	public:
		HttpRequest();
		~HttpRequest();

	public:
		std::string getArgument(size_t index) const;

	private:
		std::vector<std::string> mArgs;
};