#include "Routes/HttpRouter.hpp"

#include "Util/Message.hpp"
#include "Util/HttpUtil.hpp"

#include <algorithm>
#include <iterator>
#include <stdexcept>


HttpRouter::HttpRouter() : HttpFilterManager(), HttpErrorManager()
{

}

HttpRouter::~HttpRouter()
{

}


HttpController HttpRouter::getRequestController(const RequestWrapper& request)
{
	return getRequestController(http::to_string(request.method()), request.target());
}

HttpController HttpRouter::getRequestController(const HttpMethod& method, const std::string& path)
{
	return getRequestController(http::to_string(method), path);
}

HttpController HttpRouter::getRequestController(const std::string& method, const std::string& path)
{
	MESSAGE("\nROUTER: requesting \"", path , "\" with method \"", method, "\"\n");

	EndpointSearchData data;
	data.pathStream.str(path);
	data.endpointPtr = &mRootEndpoint;

	if(!searchChildren(data))
		return HttpController(this->getErrorCallback(http::status::not_found));

	// check method validity
	auto callbackIt = data.endpointPtr->callbacks.find(method);

	if(callbackIt == data.endpointPtr->callbacks.end())
		return HttpController(this->getErrorCallback(http::status::not_found));

	return HttpController(callbackIt->second, std::move(data.args), std::move(data.form));
}

bool HttpRouter::searchChildren(EndpointSearchData& data)
{
	auto& buffer = data.buffer;

	while(std::getline(data.pathStream, buffer, '/'))
	{
		if(buffer.empty())
			continue;

		getFormData(data);

		if(buffer.empty())
			continue;

		if(lookInFixedContainer(data))
			continue;

		return lookInArgumentContainer(data);
	}

	return true;
}

void HttpRouter::getFormData(EndpointSearchData& data)
{
	auto& buffer = data.buffer;

	const auto pos = buffer.find('?');
    
    if(pos == std::string::npos)
    	return;
    
    auto&& formText = buffer.substr(pos+1);
    auto&& newForm = HttpUtil::extractFormData(formText);

    data.form.merge(newForm);

    buffer = buffer.substr(0, pos);
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

	for(auto& [priority, argument] : argumentChildren)
	{
		if(!checkFilters(buffer, argument.endpoint))
			continue;

		data.endpointPtr = &argument.endpoint;
		
		data.args.push_back(buffer); // this helps to keep the order of the arguments
		
		if(searchChildren(data))
			return true;

		data.args.erase(data.args.end() - 1);
	}

	return false;
}


bool HttpRouter::checkFilters(const std::string& data, const HttpEndpoint& endpoint)
{
	MESSAGE("\t==> Filtering argument \"", data, "\"\n");

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
		MESSAGE("ROUTER [RUNTIME ERROR]: ", error.what(), "\n");
	}
	catch(std::exception& error)
	{
		MESSAGE("ROUTER [UNDEFINED ERROR]: ", error.what(), "\n");
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
							 [&type](const auto& pair)
							 {
							 	const auto& argument = pair.second;
							 	return argument.type == type;
							 });

	if(it == data.end())
	{
		it = data.insert(createArgumentEndpoint(type));
	}

	return &it->second.endpoint;
}

std::pair<int, HttpRouter::Argument> HttpRouter::createArgumentEndpoint(const std::string& type) const
{
	HttpEndpoint endpoint;

	const auto&& [filterData, isValid] = this->getFilterData(type);
	const auto& filters = filterData.filters;

	if(!isValid)
		throw std::runtime_error("Can't find a filter for type \"" + type + "\".");

	endpoint.filters.insert(endpoint.filters.begin(), std::make_move_iterator(filters.begin()),
													std::make_move_iterator(filters.end()));

	return std::make_pair(filterData.priority, Argument{type, std::move(endpoint)});
}
