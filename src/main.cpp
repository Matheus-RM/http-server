#include "HttpServer.hpp"

#include "fileGetters.hpp"

#define BIND_HTTP(SERVER, METHOD, PATH, FUNCTION) \
		SERVER.addEndpoint(HttpMethod::METHOD, PATH, std::bind(FUNCTION, std::placeholders::_1));

int main(int argc, char** argv)
{
	HttpServer server;
	
	server.readConfigureFile("Server.toml");

	BIND_HTTP(server, get, "/", index);
	BIND_HTTP(server, get, "/styles/<string>", getStylesheet);
	BIND_HTTP(server, get, "/scripts/<string>", getScript);
	BIND_HTTP(server, get, "/lang/<string>/<string>", getLanguage);

	server.run();

	return 0;
}