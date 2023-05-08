#include "HttpErrorManager.hpp"

#include <iostream>
#include <sstream>

#include "HttpServer.hpp"
#include "EnumHelper.hpp"

#include <inja/inja.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;


HttpErrorManager::HttpErrorManager()
{

}

HttpErrorManager::~HttpErrorManager()
{

}


void HttpErrorManager::setErrorTemplateFile(const std::string& path)
{
	mTemplatePath = path;
}

void HttpErrorManager::setErrorCallback(http::status status, HttpCallback callback)
{
	if(mDefinedCallbacks.find(status) != mDefinedCallbacks.end())
		std::cout << "WARNING: resetting status [" << status << "] callback\n";

	mDefinedCallbacks[status] = callback;
}


HttpCallback HttpErrorManager::getErrorCallback(http::status status)
{
	auto it = mDefinedCallbacks.find(status);

	if(it != mDefinedCallbacks.end())
		return it->second;

	auto callback = [status, &path = mTemplatePath](HttpServer* server, const HttpArgs& args) -> HttpResponse
	{
		auto [injaEnvironment, isValid] = server->getDependency<inja::Environment>();

		if(!isValid)
			return HttpErrorManager::getDefaultErrorResponse(status, path);

		json data;
		data["error_reason"] = http::obsolete_reason(status);
		data["error_number"] = to_underlying(status);

		std::cout << "\t==> Generating response for error " << status << "\n";

		return HttpResponse(injaEnvironment.render_file(path, data), status, "text/html");
	};

	return callback;
}

HttpResponse HttpErrorManager::getErrorResponse(http::status status, HttpServer* server, const HttpArgs& args)
{
	auto callback = getErrorCallback(status);

	return callback(server, args);
}

HttpResponse HttpErrorManager::getDefaultErrorResponse(http::status status, const std::string& path)
{
	std::ostringstream data;
	data << "An unexpected error occurried.\n";
	data << "-> Error: " << to_underlying(status) << " - " << http::obsolete_reason(status) << ".\n";

	return HttpResponse(data.str(), status, "text/plain");
}

