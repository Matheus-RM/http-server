#pragma once

#include <functional>
#include <any>
#include <unordered_map>
#include <string>

using HttpCaster = std::function<std::any(const std::string&)>;

class HttpTypeManager
{
	public:
		HttpTypeManager();
		virtual ~HttpTypeManager();

	public:


	private:

};