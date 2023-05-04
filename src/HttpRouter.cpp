#include "HttpRouter.hpp"

#include <algorithm>
#include <iostream>

HttpRouter::HttpRouter()
{

}

HttpRouter::~HttpRouter()
{

}


void HttpRouter::addEndpoint(const HttpMethod& method, const std::string& path, HttpCallback callback)
{
	auto& endpoint = gotoEndpoint(path);

	endpoint.callbacks.emplace(http::to_string(method), callback);
}

HttpController HttpRouter::getRequest(const http::request<http::dynamic_body>& request)
{
	return getRequest(http::to_string(request.method()), request.target());
}

HttpController HttpRouter::getRequest(const HttpMethod& method, const std::string& path)
{
	return getRequest(http::to_string(method), path);
}

HttpController HttpRouter::getRequest(const std::string& method, const std::string& path)
{
	std::cout << "Router: requesting \"" << path << "\", with method \"" << method << "\".\n";

	EndpointSearchData data;
	data.pathStream.str(path);
	data.endpointPtr = &mRootEndpoint;

	if(!searchChildren(data))
		return std::bind(HttpRouter::notFoundError, std::placeholders::_1);

	// check method validity
	auto callbackIt = data.endpointPtr->callbacks.find(method);

	if(callbackIt == data.endpointPtr->callbacks.end())
		return std::bind(HttpRouter::notFoundError, std::placeholders::_1);

	return HttpController(callbackIt->second, std::move(data.args));
}

bool HttpRouter::searchChildren(EndpointSearchData& data)
{
	auto& buffer = data.buffer;

	while(std::getline(data.pathStream, buffer, '/'))
	{
		if(buffer.empty())
			continue;

		if(lookInFixedContainer(data))
			continue;

		return lookInArgumentContainer(data);
	}

	return true;
}

bool HttpRouter::lookInFixedContainer(EndpointSearchData& data)
{
	auto& buffer = data.buffer;
	auto& fixedChildren = data.endpointPtr->fixedChildren;

	auto it = fixedChildren.find(buffer);
		
	if(it == fixedChildren.end())
		return false;
	
	data.endpointPtr = &it->second;
	return true;
}

bool HttpRouter::lookInArgumentContainer(EndpointSearchData& data)
{
	auto& buffer = data.buffer;
	auto& argumentChildren = data.endpointPtr->argumentChildren;	

	if(argumentChildren.empty())
		return false;

	for(auto& [name, endpoint] : argumentChildren)
	{
		if(!checkFilters(buffer, endpoint))
			continue;

		data.endpointPtr = &endpoint;
		
		data.args.push_back(buffer); // this helps to keep the order of the arguments
		
		if(searchChildren(data))
			return true;

		data.args.erase(data.args.end() - 1);
	}

	return false;
}


bool HttpRouter::checkFilters(const std::string& data, const HttpEndpoint& endpoint)
{
	for(const auto filter : endpoint.filters)
	{
		if(!filter(data))
			return false;
	}

	return true;
}


HttpRouter::HttpEndpoint& HttpRouter::gotoEndpoint(const std::string& path)
{
	std::stringstream pathStream(path);
	std::string buffer;

	auto* endpointPtr = &mRootEndpoint;

	while(std::getline(pathStream, buffer, '/'))
	{
		if(buffer.empty())
			continue;

		if(buffer.front() == '<' && buffer.back() == '>') // is argument
		{
			endpointPtr = createIfDontExist(buffer, endpointPtr->argumentChildren);
			continue;
		}

		endpointPtr = createIfDontExist(buffer, endpointPtr->fixedChildren);
	}

	return *endpointPtr;
}

HttpRouter::HttpEndpoint* HttpRouter::createIfDontExist(const std::string& value, FixedContainer& data)
{
	auto it = data.find(value);

	if(it == data.end())
	{
		it = data.emplace(value, HttpEndpoint()).first;
	}

	return &it->second;
}

HttpRouter::HttpEndpoint* HttpRouter::createIfDontExist(const std::string& value, ArgumentContainer& data)
{
	auto it = std::find_if(data.begin(), data.end(),
							 [&value](auto& pair)
							 {
							 	return pair.first == value;
							 });

	if(it == data.end())
	{
		data.emplace_back(value, HttpEndpoint());
		it = data.end() - 1;

		if(value == "<int>")
			it->second.filters.push_back(std::bind(HttpRouter::checkInteger, std::placeholders::_1));

		else if(value == "<uint>")
			it->second.filters.push_back(std::bind(HttpRouter::checkUnsigned, std::placeholders::_1));
	}

	return &it->second;
}

HttpResponse HttpRouter::notFoundError(const std::vector<std::string>& args)
{
	return HttpResponse(std::ostringstream("404 - Not Found."), http::status::bad_request, "text/plain");
}

bool HttpRouter::checkInteger(const std::string& text)
{
	if(text.empty())
		return false;

	if(!isdigit(text[0]) && text[0] != '-' && text[0] != '+')
		return false;
	
	return (text.substr(1).find_first_not_of("0123456789") == std::string::npos);
}

bool HttpRouter::checkUnsigned(const std::string& text)
{
	if(text.empty())
		return false;

	if(!isdigit(text[0]) && text[0] != '+')
		return false;
	
	return (text.substr(1).find_first_not_of("0123456789") == std::string::npos);
}
