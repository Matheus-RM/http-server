#pragma once

#include "Base/HttpConnection.hpp"
#include "Base/DependencyManager.hpp"
#include "Util/HttpHelper.hpp"
#include "Routes/HttpRouter.hpp"

#include <string>

#ifdef _DEBUG
	#define BUILD_MODE "debug"

#elif defined NDEBUG
	#define BUILD_MODE "release"
	
#else
	#define BUILD_MODE "not-know"

#endif

#define BIND_HTTP(SERVER, METHOD, PATH, FUNCTION) \
		SERVER.addEndpoint(HttpMethod::METHOD, PATH, \
			std::bind(FUNCTION, std::placeholders::_1,  std::placeholders::_2));

class HttpServer : public HttpRouter, public DependencyManager
{
	public:
		HttpServer();
		~HttpServer();

	private:
		struct Configuration
		{
			std::string name = "Theu-Server";
			std::string ipAddress = "127.0.0.1";
			unsigned short port = 8080;
			std::string templatePath = "templates/";
			std::string errorTemplate = "error.html";
		};

	public:
		void readConfigureFile(const std::string& path);

		std::string getName() const;

		void run();

	private:
		void configure();

		void acceptConnection();
		void handleConnection(const beast::error_code& error);

	private:
		asio::io_context mContext;
		tcp::acceptor mAcceptor;
		tcp::socket mConnectionSocket;

		Configuration mConfigure;

		bool mShouldQuit = false;
};