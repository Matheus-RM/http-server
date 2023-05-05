#include "fileGetters.hpp"

#include <fstream>

std::string readFile(const std::string& path)
{
	std::ifstream file(path);
	std::ostringstream body;
	body << file.rdbuf();
	file.close();

	return body.str();
}

HttpResponse index(const std::vector<std::string>& args)
{
	return HttpResponse(readFile("resources/index.html"), http::status::ok, "text/html");
}

HttpResponse getStylesheet(const std::vector<std::string>& args)
{
	return HttpResponse(readFile("resources/styles/" + args[0]), http::status::ok, "text/css");
}

HttpResponse getScript(const std::vector<std::string>& args)
{
	return HttpResponse(readFile("resources/scripts/" + args[0]), http::status::ok, "text/javascript");
}

HttpResponse getLanguage(const std::vector<std::string>& args)
{
	return HttpResponse(readFile("resources/lang/" + args[0] + "/" + args[1]),
						http::status::ok, "application/json");
}
