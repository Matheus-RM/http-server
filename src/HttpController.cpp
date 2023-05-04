#include "HttpController.hpp"

#include <boost/beast/core.hpp>

namespace beast = boost::beast;

HttpController::HttpController(HttpCallback callback) : mCallback(callback)
{

}

HttpController::HttpController(HttpCallback callback, const std::vector<std::string>& args)
								: mCallback(callback), mArgs(std::move(args))
{

}

http::response<http::dynamic_body> HttpController::execute() const
{
	http::response<http::dynamic_body> result;	

	const auto&& data = mCallback(mArgs);

	beast::ostream(result.body()) << data.body.str();

	result.result(data.status);
	result.set(http::field::content_type, data.type);
	result.content_length(result.body().size());

	return result;
}