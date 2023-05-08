#include "HttpController.hpp"
#include "HttpServer.hpp"

#include <boost/beast/core.hpp>

#include <iostream>

namespace beast = boost::beast;

HttpController::HttpController(HttpCallback callback) : mCallback(callback)
{

}

HttpController::HttpController(HttpCallback callback, const HttpArgs& args)
								: mCallback(callback), mArgs(std::move(args))
{

}

http::response<http::dynamic_body> HttpController::execute(HttpServer* server) const
{
	http::response<http::dynamic_body> result;	

	std::cout << "\t==> Retrieving data\n";
	const auto&& data = mCallback(server, mArgs);

	for(const auto& [field, value] : data.fieldsValues)
	{
		std::cout << "\t==> Setting field " << http::to_string(field) << " to " << value << "\n";
		result.set(field, value);
	}

	beast::ostream(result.body()) << data.body;

	result.result(data.status);
	//result.set(http::field::content_type, data.type);
	//result.content_length(result.body().size());

	std::cout << "\t==> Returning response\n";

	return result;
}