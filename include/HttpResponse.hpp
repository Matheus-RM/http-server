#pragma once

#include <boost/beast/http.hpp>
#include <string>
#include <unordered_map>

#include "EnumHelper.hpp"

namespace http = boost::beast::http;

class HttpResponse
{
	public:
		HttpResponse(const http::status& status);
		HttpResponse(const std::string& data, const http::status& status, const std::string& type);

		~HttpResponse();

		friend class HttpController;

	public:
		void setField(http::field field, const std::string& value);
		void setBody(const std::string& data, const std::string& type);

	private:
		std::string body;
		http::status status;
		std::unordered_map<http::field, std::string, EnumClassHash> fieldsValues;
};