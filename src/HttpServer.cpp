#include "HttpServer.hpp"

#include <functional>
#include <iostream>
#include <memory>

HttpServer::HttpServer()
	 : mContext(1), mAcceptor(mContext), mConnectionSocket(mContext), mRouterPtr(new HttpRouter())
{

	const auto endpoint = tcp::endpoint(asio::ip::make_address(mIpAddress), mPort);
	mAcceptor = tcp::acceptor(mContext, endpoint);
}

HttpServer::~HttpServer()
{
	std::cout << "Server: the server is now closing.\n";
}


void HttpServer::run()
{
	std::cout << "Server: server is now running.\n";
	std::cout << "Server: address=\"" << mIpAddress << "\", port=" << mPort << ".\n";

	acceptConnection();

	mContext.run();
}

void HttpServer::acceptConnection()
{
	mAcceptor.async_accept(mConnectionSocket,
				std::bind(&HttpServer::handleConnection, this, std::placeholders::_1)
			);
}

void HttpServer::handleConnection(const beast::error_code& error)
{
	if(!error)
    {
        std::cout << "Server: found new connection.\n";

        auto connection = std::make_shared<HttpConnection>(std::move(mConnectionSocket), mRouterPtr);
        connection->start();
    }

	acceptConnection();
}


void HttpServer::addEndpoint(const HttpMethod& method, const std::string& path, HttpCallback callback)
{
	mRouterPtr->addEndpoint(method, path, callback);
}
