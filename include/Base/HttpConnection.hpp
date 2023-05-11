#pragma once

#include "Util/HttpHelper.hpp"
#include "Routes/HttpRouter.hpp"
#include <chrono>
#include <memory>

class HttpServer;

class HttpConnection : public std::enable_shared_from_this<HttpConnection>
{
	public:
		HttpConnection(tcp::socket socket, HttpServer* server);
		~HttpConnection();

	public:

		using ptr = std::shared_ptr<HttpConnection>;

		void start();

	private:
		void readRequest();
		void processRequest();
		void writeResponse();
		void checkDeadline();

		void handleRequest(const beast::error_code& error, size_t bytesTransfered);
		void handleResponse(beast::error_code error, size_t bytesTransfered);
		void handleDeadline(beast::error_code error);

	private:
		tcp::socket mSocket;
		beast::flat_buffer mBuffer {8192};
		http::request<http::dynamic_body> mRequest;
		http::response<http::dynamic_body> mResponse;
		asio::steady_timer mDeadline {mSocket.get_executor(), std::chrono::seconds(60)};

		HttpServer* mServerPtr;
};
