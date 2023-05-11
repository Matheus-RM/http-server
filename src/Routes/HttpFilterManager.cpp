#include "Routes/HttpFilterManager.hpp"

#include "Routes/Filters.hpp"

HttpFilterManager::HttpFilterManager()
{
	addFilter("string", BIND_FILTER(Filters::checkText), -10);
	addFilter("int", BIND_FILTER(Filters::checkInteger), -2);
	addFilter("uint", BIND_FILTER(Filters::checkUnsigned), -1);
}

HttpFilterManager::~HttpFilterManager()
{

}


void HttpFilterManager::addFilter(const std::string& type, HttpFilter filter, int priority)
{
	addFilters(type, {filter}, priority);
}

void HttpFilterManager::addFilters(const std::string& type, const std::vector<HttpFilter>& filters, int priority)
{
	const auto&& typeName = getTypeName(type);

	auto it = mFilters.find(typeName);

	if(it == mFilters.end())
	{
		it = mFilters.emplace(std::make_pair(typeName, FilterData())).first;
		it->second.priority = priority;
	}

	auto& filtersContainer = it->second.filters;
	filtersContainer.insert(filtersContainer.end(), filters.begin(), filters.end());
}

auto HttpFilterManager::getFilters(const std::string& type) const -> std::pair<std::vector<HttpFilter>, bool>
{
	const auto&& typeName = getTypeName(type);

	auto it = mFilters.find(typeName);

	if(it == mFilters.end())
	{
		return std::make_pair(std::vector<HttpFilter>(), false);
	}

	auto& filtersContainer = it->second.filters;

	return std::make_pair(filtersContainer, true);
}

std::string HttpFilterManager::getTypeName(const std::string& type) const
{
	if(type.front() == '<' && type.back() == '>')
		return type.substr(1, type.size() - 2);

	return type;
}

auto HttpFilterManager::getTypePriority(const std::string& type) const -> std::pair<int, bool>
{
	const auto&& typeName = getTypeName(type);
	auto it = mFilters.find(typeName);

	if(it == mFilters.end())
	{
		return std::make_pair(0, false);
	}

	return std::make_pair(it->second.priority, true);
}

auto HttpFilterManager::getFilterData(const std::string& type) const -> std::pair<FilterData, bool>
{
	const auto&& typeName = getTypeName(type);
	auto it = mFilters.find(typeName);

	if(it == mFilters.end())
	{
		return std::make_pair(FilterData(), false);
	}

	return std::make_pair(it->second, true);
}
