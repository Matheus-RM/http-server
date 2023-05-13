#include "Base/HttpController.hpp"

#include "Base/HttpServer.hpp"
#include "Util/Message.hpp"
#include "Util/HttpUtil.hpp"

#include <sstream>
#include <iomanip>

#include <boost/beast/core.hpp>
#include <boost/beast/http/verb.hpp>

namespace beast = boost::beast;


HttpController::HttpController(HttpCallback callback) : mCallback(callback)
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

HttpController::HttpController(HttpCallback callback, const HttpRequest::Args& args)
								: mCallback(callback), mArgs(args)
{

}

HttpController::HttpController(HttpCallback callback, const HttpRequest::Args& args,
								const HttpRequest::FormData& form)
								: mCallback(callback), mArgs(args), mFormData(form)
{

}



ResponseWrapper HttpController::execute(HttpServer* server, const RequestWrapper& request)
{
	ResponseWrapper result;	

	MESSAGE("\t==> Retrieving response data\n");
	const auto&& data = mCallback(server, generateRequest(request));

	for(const auto& [field, value] : data.fieldsValues)
	{
		MESSAGE("\t\t==> Setting field ", http::to_string(field), " to ", value, "\n");
		result.set(field, value);
	}

	beast::ostream(result.body()) << data.body;

	result.result(data.status);

	MESSAGE("\t==> Returning response\n");

	return result;
}

HttpRequest HttpController::generateRequest(const RequestWrapper& request)
{
	if(request.method() == http::verb::post)
	{
		getRequestBodyData(request);
	}

	return HttpRequest(std::move(mArgs), std::move(mFormData));
}

void HttpController::getRequestBodyData(const RequestWrapper& request)
{
	MESSAGE("\t\t==> Getting Form Data\n");

	auto& body = request.body();

	if(body.empty())
	{
		MESSAGE("\t\t==> Form data was not found inside the request body\n");
		return;
	}

	mFormData.merge(HttpUtil::extractFormData(body));
}
