#include "HttpConnection.hpp"

#include <functional>

HttpConnection::HttpConnection(tcp::socket socket, HttpRouter::Ptr router)
							 : mSocket(std::move(socket)), mRouterPtr(router)
{

}

HttpConnection::~HttpConnection()
{

}


tcp::socket& HttpConnection::getSocket()
{
	return mSocket;
}


void HttpConnection::start()
{
	readRequest();
	checkDeadline();
}


void HttpConnection::readRequest()
{
	http::async_read(mSocket, mBuffer, mRequest,
		std::bind(&HttpConnection::handleRequest, shared_from_this(),
					std::placeholders::_1, std::placeholders::_2)
		);
}

void HttpConnection::handleRequest(const beast::error_code& error, size_t bytesTransfered)
{
	boost::ignore_unused(bytesTransfered);

	if(error)
		return;

	processRequest();
}

void HttpConnection::processRequest()
{
	const auto&& controller = mRouterPtr->getRequest(mRequest);
	mResponse = controller.execute();

	mResponse.version(mRequest.version());
	mResponse.keep_alive(false);
	mResponse.set(http::field::server, "TheuServer");

	//mRequest.method_string()
	writeResponse();
}

void HttpConnection::writeResponse()
{
	http::async_write(mSocket, mResponse,
		std::bind(&HttpConnection::handleResponse, shared_from_this(),
					std::placeholders::_1, std::placeholders::_2)
		);
}

void HttpConnection::handleResponse(beast::error_code error, size_t bytesTransfered)
{
	mSocket.shutdown(tcp::socket::shutdown_send, error);
	mDeadline.cancel();
}

void HttpConnection::checkDeadline()
{
	mDeadline.async_wait(std::bind(&HttpConnection::handleDeadline, shared_from_this(),
										std::placeholders::_1));
}

void HttpConnection::handleDeadline(beast::error_code error)
{
	if(!error)
	{
		mSocket.close(error);
	}
}
