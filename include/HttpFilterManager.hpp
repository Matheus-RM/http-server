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

	public:
		void addFilter(const std::string& type, HttpFilter filter);
		void addFilters(const std::string& type, const std::vector<HttpFilter>& filters);
		auto getFilters(const std::string& type) const -> std::pair<std::vector<HttpFilter>, bool>;

	private:
		std::string getTypeName(const std::string& type) const;

	private:
		std::unordered_map<std::string, std::vector<HttpFilter>> mFilters;
};