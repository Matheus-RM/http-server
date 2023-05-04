#pragma once

#include <vector>
#include <string>
#include <functional>

#include "HttpResponse.hpp"

using HttpCallback = std::function<HttpResponse(const std::vector<std::string>&)>;

class HttpController
{
	public:
		HttpController(HttpCallback callback);

		HttpController(HttpCallback callback, const std::vector<std::string>& args);

		http::response<http::dynamic_body> execute() const;

	private:
		HttpCallback mCallback;
		std::vector<std::string> mArgs;
};