#pragma once

#include "HttpCallback.hpp"

class HttpController
{
	public:
		HttpController(HttpCallback callback);

		HttpController(HttpCallback callback, const HttpArgs& args);

		http::response<http::dynamic_body> execute(HttpServer* server) const;

	private:
		HttpCallback mCallback;
		HttpArgs mArgs;
};