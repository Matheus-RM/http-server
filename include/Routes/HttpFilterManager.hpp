#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <functional>

using HttpFilter = std::function<bool(const std::string&)>;

class HttpFilterManager
{
	public:
		HttpFilterManager();
		virtual ~HttpFilterManager();

		struct FilterData
		{
			std::vector<HttpFilter> filters;
			int priority;
		};

	public:
		void addFilter(const std::string& type, HttpFilter filter, int priority);
		void addFilters(const std::string& type, const std::vector<HttpFilter>& filters, int priority);
		auto getFilters(const std::string& type) const -> std::pair<std::vector<HttpFilter>, bool>;

		auto getTypePriority(const std::string& type) const -> std::pair<int, bool>;

		auto getFilterData(const std::string& type) const -> std::pair<FilterData, bool>;

	private:
		std::string getTypeName(const std::string& type) const;

	private:
		std::unordered_map<std::string, FilterData> mFilters;
};