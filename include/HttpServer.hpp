#pragma once

#include "HttpHelper.hpp"
#include "HttpConnection.hpp"
#include "HttpRouter.hpp"

#include <string>
#include <memory>

class HttpServer
{
	public:
		HttpServer();
		~HttpServer();

	public:
		void addEndpoint(const HttpMethod& method, const std::string& path, HttpCallback callback);

		void run();

	private:
		void acceptConnection();
		void handleConnection(const beast::error_code& error);

	private:
		asio::io_context mContext;
		tcp::acceptor mAcceptor;
		tcp::socket mConnectionSocket;

		// this will be in a file
		std::string mServerName = "TheuServer";
		std::string mIpAddress = "127.0.0.1";
		std::string mIpVersion = "IPv4";
		unsigned short mPort = 8080;

		HttpRouter::Ptr mRouterPtr;
};