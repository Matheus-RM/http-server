#include "Base/HttpServer.hpp"

#include "fileGetters.hpp"

int main(int argc, char** argv)
{
	HttpServer server;
	
	server.readConfigureFile("Server.toml");

	BIND_HTTP(server, get, "/", index);
	BIND_HTTP(server, get, "/styles/<string>", getStylesheet);
	BIND_HTTP(server, get, "/scripts/<string>", getScript);
	BIND_HTTP(server, get, "/lang/<string>/<string>", getLanguage);
	BIND_HTTP(server, get, "/hello/<string>", sayHello);
	BIND_HTTP(server, get, "/redirect", redirect);

	BIND_HTTP(server, get, "/test/<string>", argumentTestString);
	BIND_HTTP(server, get, "/test/<int>", argumentTestInt);

	server.run();

	return 0;
}