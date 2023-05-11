#include "Base/HttpController.hpp"

#include "Base/HttpServer.hpp"
#include "Util/Message.hpp"

#include <boost/beast/core.hpp>

namespace beast = boost::beast;


HttpController::HttpController(HttpCallback callback) : mCallback(callback)
{

}

HttpController::HttpController(HttpCallback callback, const HttpRequest::Args& args)
								: mCallback(callback), mArgs(args)
{

}

HttpController::HttpController(HttpCallback callback, const HttpRequest::Args& args,
								const HttpRequest::FormData& form)
								: mCallback(callback), mArgs(args), mFormData(form)
{

}


HttpController::HttpController(HttpCallback callback, HttpRequest::Args&& args)
								: mCallback(callback), mArgs(std::move(args))
{

}

HttpController::HttpController(HttpCallback callback, HttpRequest::Args&& args, HttpRequest::FormData&& form)
								: mCallback(callback), mArgs(std::move(args)), mFormData(std::move(form))
{

}


http::response<http::dynamic_body> HttpController::execute(HttpServer* server) const
{
	http::response<http::dynamic_body> result;	

	MESSAGE("\t==> Retrieving data\n");
	const auto&& data = mCallback(server, generateRequest());

	for(const auto& [field, value] : data.fieldsValues)
	{
		MESSAGE("\t==> Setting field ", http::to_string(field), " to ", value, "\n");
		result.set(field, value);
	}

	beast::ostream(result.body()) << data.body;

	result.result(data.status);
	//result.set(http::field::content_type, data.type);
	//result.content_length(result.body().size());

	MESSAGE("\t==> Returning response\n");

	return result;
}

HttpRequest HttpController::generateRequest() const
{
	if(mFormData.empty())
	{
		// ...
	}

	return HttpRequest(std::move(mArgs), std::move(mFormData));
}
