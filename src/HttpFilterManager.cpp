#include "HttpFilterManager.hpp"

#include "Filters.hpp"

HttpFilterManager::HttpFilterManager()
{
	addFilter("string", BIND_FILTER(Filters::checkText));
	addFilter("int", BIND_FILTER(Filters::checkInteger));
	addFilter("uint", BIND_FILTER(Filters::checkUnsigned));
}

HttpFilterManager::~HttpFilterManager()
{

}


void HttpFilterManager::addFilter(const std::string& type, HttpFilter filter)
{
	addFilters(type, {filter});
}

void HttpFilterManager::addFilters(const std::string& type, const std::vector<HttpFilter>& filters)
{
	const auto&& typeName = getTypeName(type);

	auto it = mFilters.find(typeName);

	if(it == mFilters.end())
	{
		mFilters.insert(std::make_pair(typeName, filters));
		return;
	}

	it->second.insert(it->second.end(), filters.begin(), filters.end());
}

auto HttpFilterManager::getFilters(const std::string& type) const -> std::pair<std::vector<HttpFilter>, bool>
{
	const auto&& typeName = getTypeName(type);

	auto it = mFilters.find(typeName);

	if(it == mFilters.end())
	{
		return std::make_pair(std::vector<HttpFilter>(), false);
	}

	return std::make_pair(it->second, true);
}

std::string HttpFilterManager::getTypeName(const std::string& type) const
{
	if(type.front() == '<' && type.back() == '>')
		return type.substr(1, type.size() - 2);

	return type;
}
