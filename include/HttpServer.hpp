#pragma once

#include "HttpHelper.hpp"
#include "HttpConnection.hpp"
#include "HttpRouter.hpp"

#include <string>
#include <memory>

#include <inja/inja.hpp>

#ifdef _DEBUG
	
	#define BUILD_MODE "debug"

#elif defined NDEBUG

	#define BUILD_MODE "release"
	
#else

	#define BUILD_MODE "debug"

#endif

class HttpServer : public HttpRouter
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

		std::shared_ptr<inja::Environment> mTemplateEnvironmentPtr;
};