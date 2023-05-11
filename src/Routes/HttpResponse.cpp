#include "Routes/HttpResponse.hpp"

HttpResponse::HttpResponse(const http::status& status) : status(status)
{
	fieldsValues.emplace(http::field::content_type, "text/plain");
}

HttpResponse::HttpResponse(const std::string& data, const http::status& status, const std::string& type)
			: body(data), status(status)
{
	setBody(data, type);
}

HttpResponse::~HttpResponse()
{

}

void HttpResponse::setField(http::field field, const std::string& value)
{
	auto it = fieldsValues.find(field);

	if(it == fieldsValues.end())
	{
		fieldsValues.emplace(field, value);
		return;
	}

	it->second = value;
}

void HttpResponse::setBody(const std::string& data, const std::string& type)
{
	fieldsValues.emplace(http::field::content_length, std::to_string(data.size()));
	fieldsValues.emplace(http::field::content_type, type);
}