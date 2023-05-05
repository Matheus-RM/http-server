#pragma once

#include <boost/beast/http.hpp>
#include <string>
#include <sstream>

namespace http = boost::beast::http;

struct HttpResponse
{
	HttpResponse(const std::string& data, const http::status& status, const std::string& type)
				: body(data), status(status), type(type) {}

	std::string body;
	http::status status;
	std::string type;
};