#pragma once

#include "Routes/HttpCallback.hpp"

class HttpController
{
	public:
		HttpController(HttpCallback callback);

		HttpController(HttpCallback callback, const HttpRequest::Args& args);
		HttpController(HttpCallback callback, const HttpRequest::Args& args, const HttpRequest::FormData& form);

		HttpController(HttpCallback callback, HttpRequest::Args&& args);
		HttpController(HttpCallback callback, HttpRequest::Args&& args, HttpRequest::FormData&& form);

		http::response<http::dynamic_body> execute(HttpServer* server) const;

	private:
		HttpRequest generateRequest() const;

	private:
		HttpCallback mCallback;
		HttpRequest::Args mArgs;
		HttpRequest::FormData mFormData;
};