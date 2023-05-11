#include "fileGetters.hpp"

#include <fstream>

#include <inja/inja.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::string readFile(const std::string& path)
{
	std::ifstream file(path);
	std::ostringstream body;
	body << file.rdbuf();
	file.close();

	return body.str();
}

HttpResponse index(HttpServer* server, const HttpRequest& request)
{
	return HttpResponse(readFile("resources/index.html"),
						http::status::ok, "text/html");
}

HttpResponse getStylesheet(HttpServer* server, const HttpRequest& request)
{
	return HttpResponse(readFile("resources/styles/" + request.getArgument(0)), 
						http::status::ok, "text/css");
}

HttpResponse getScript(HttpServer* server, const HttpRequest& request)
{
	return HttpResponse(readFile("resources/scripts/" + request.getArgument(0)), 
						http::status::ok, "application/javascript");
}

HttpResponse getLanguage(HttpServer* server, const HttpRequest& request)
{
	return HttpResponse(readFile("resources/lang/" + request.getArgument(0) + "/" + request.getArgument(1)),
						http::status::ok, "application/json");
}

HttpResponse sayHello(HttpServer* server, const HttpRequest& request)
{
	auto [injaEnvironment, isValid] = server->getDependency<inja::Environment>();

	if(!isValid)
		return server->getErrorResponse(http::status::internal_server_error, server, request);

	json data;
	data["name"] = request.getArgument(0);

	return HttpResponse(injaEnvironment.render_file("hello.html", data), http::status::ok, "text/html");
}

HttpResponse redirect(HttpServer* server, const HttpRequest& request)
{
	HttpResponse response(http::status::moved_permanently);

	response.setField(http::field::location, "/");

	return response;
}

HttpResponse argumentTestInt(HttpServer* server, const HttpRequest& request)
{
	return HttpResponse("The given argument \"" + request.getArgument(0) + "\" is a integer.",
						http::status::ok, "text/plain");
}

HttpResponse argumentTestString(HttpServer* server, const HttpRequest& request)
{
	return HttpResponse("The given argument \"" + request.getArgument(0) + "\" is a string.",
						http::status::ok, "text/plain");
}