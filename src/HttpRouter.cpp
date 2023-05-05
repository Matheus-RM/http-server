#include "HttpRouter.hpp"

#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <iostream>

HttpRouter::HttpRouter() : HttpFilterManager()
{

}

HttpRouter::~HttpRouter()
{

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
	std::cout << "\nROUTER: requesting \"" << path << "\" with method \"" << method << "\"\n";

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
	std::cout << "\t-> Checking filters for argument \"" << data << "\"\n";

	for(const auto& filter : endpoint.filters)
	{
		if(!filter(data))
			return false;
	}

	return true;
}

void HttpRouter::addEndpoint(const HttpMethod& method, const std::string& path, HttpCallback callback)
{
	try
	{
		auto& endpoint = gotoEndpoint(path);

		endpoint.callbacks.emplace(http::to_string(method), callback);
	}
	catch(std::runtime_error& error)
	{
		std::cout << "ROUTER [RUNTIME ERROR]: " << error.what() << "\n";
	}
	catch(std::exception& error)
	{
		std::cout << "ROUTER [UNDEFINED ERROR]: " << error.what() << "\n";
	}
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

HttpRouter::HttpEndpoint* HttpRouter::createIfDontExist(const std::string& type, ArgumentContainer& data)
{
	auto it = std::find_if(data.begin(), data.end(),
							 [&type](auto& pair)
							 {
							 	return pair.first == type;
							 });

	if(it == data.end())
	{
		it = data.emplace(data.end(), type, HttpEndpoint());

		addFiltersToEndpoint(it->second, type);
	}

	return &it->second;
}

void HttpRouter::addFiltersToEndpoint(HttpRouter::HttpEndpoint& endpoint, const std::string& type)
{
	const auto&& [filters, state] = this->getFilters(type);

	if(!state)
		throw std::runtime_error("Can't find a filter for type \"" + type + "\".");

	endpoint.filters.insert(endpoint.filters.begin(), std::make_move_iterator(filters.begin()),
													std::make_move_iterator(filters.end()));
}


HttpResponse HttpRouter::notFoundError(const std::vector<std::string>& args)
{
	return HttpResponse("404 - Not Found.", http::status::not_found, "text/plain");
}