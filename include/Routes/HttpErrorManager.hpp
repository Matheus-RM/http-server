#pragma once

#include <boost/beast/http.hpp>
#include <unordered_map>

#include "Routes/HttpCallback.hpp"

namespace http = boost::beast::http;

class HttpServer;

class HttpErrorManager
{
	public:
		HttpErrorManager();
		virtual ~HttpErrorManager();

	public:

		void setErrorTemplateFile(const std::string& relativePath); // relative to the template dir
		void setErrorCallback(http::status status, HttpCallback callback);

		HttpCallback getErrorCallback(http::status status);
		HttpResponse getErrorResponse(http::status status, HttpServer* server, const HttpRequest& request);

		static HttpResponse getDefaultErrorResponse(http::status status, const std::string& path);

	private:
		std::string mTemplatePath;
		std::unordered_map<http::status, HttpCallback> mDefinedCallbacks;
};