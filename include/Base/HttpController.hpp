#pragma once

#include "Routes/HttpCallback.hpp"

#include <boost/beast/http/message.hpp>
#include <boost/beast/http/basic_dynamic_body.hpp>

namespace http = boost::beast::http;
using RequestWrapper = http::request<http::string_body>;
using ResponseWrapper = http::response<http::dynamic_body>;

class HttpController
{
	public:
		HttpController(HttpCallback callback);

		HttpController(HttpCallback callback, HttpRequest::Args&& args);
		HttpController(HttpCallback callback, HttpRequest::Args&& args, HttpRequest::FormData&& form);

		HttpController(HttpCallback callback, const HttpRequest::Args& args);
		HttpController(HttpCallback callback, const HttpRequest::Args& args, const HttpRequest::FormData& form);

		ResponseWrapper execute(HttpServer* server, const RequestWrapper& request);

	private:
		HttpRequest generateRequest(const RequestWrapper& request);
		void getRequestBodyData(const RequestWrapper& request);

	private:
		HttpCallback mCallback;
		HttpRequest::Args mArgs;
		HttpRequest::FormData mFormData;
};