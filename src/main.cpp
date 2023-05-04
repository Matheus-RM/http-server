#include "HttpServer.hpp"

#include <fstream>

int main(int argc, char** argv)
{
	HttpServer server;

	server.addEndpoint(HttpMethod::get, "/", 
			[](const auto& args) -> HttpResponse
			{
				std::ifstream file("../index.html");
				std::ostringstream body;
				body << file.rdbuf();
				file.close();

				return HttpResponse(body, http::status::ok, "text/html");
			});

	server.run();

	return 0;
}