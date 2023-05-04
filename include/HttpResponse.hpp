#pragma once

#include <boost/beast/http.hpp>
#include <string>
#include <sstream>

namespace http = boost::beast::http;

struct HttpResponse
{
	HttpResponse() {}
	HttpResponse(const std::string& data, const http::status& status, const std::string& type)
				: body(data), status(status), type(type) {}

	HttpResponse(const std::ostringstream& data, const http::status& status, const std::string& type)
				: body(data.str()), status(status), type(type) {}

	std::ostringstream body;
	http::status status;
	std::string type;
};