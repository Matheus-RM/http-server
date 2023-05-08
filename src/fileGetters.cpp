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

HttpResponse index(HttpServer* server, const HttpArgs& args)
{
	return HttpResponse(readFile("resources/index.html"), http::status::ok, "text/html");
}

HttpResponse getStylesheet(HttpServer* server, const HttpArgs& args)
{
	return HttpResponse(readFile("resources/styles/" + args[0]), http::status::ok, "text/css");
}

HttpResponse getScript(HttpServer* server, const HttpArgs& args)
{
	return HttpResponse(readFile("resources/scripts/" + args[0]), http::status::ok, "application/javascript");
}

HttpResponse getLanguage(HttpServer* server, const HttpArgs& args)
{
	return HttpResponse(readFile("resources/lang/" + args[0] + "/" + args[1]),
						http::status::ok, "application/json");
}

HttpResponse sayHello(HttpServer* server, const HttpArgs& args)
{
	auto [injaEnvironment, isValid] = server->getDependency<inja::Environment>();

	if(!isValid)
		return server->getErrorResponse(http::status::internal_server_error, server, args);

	json data;
	data["name"] = args[0];

	return HttpResponse(injaEnvironment.render_file("hello.html", data), http::status::ok, "text/html");
}

HttpResponse redirect(HttpServer* server, const HttpArgs& args)
{
	HttpResponse response(http::status::moved_permanently);

	response.setField(http::field::location, "/");

	return response;
}
