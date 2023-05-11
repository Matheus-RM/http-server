#pragma once

#include "Base/HttpServer.hpp"
#include "Routes/HttpResponse.hpp"
#include "Routes/HttpCallback.hpp"

std::string readFile(const std::string& path);

HttpResponse index(HttpServer* server, const HttpRequest& request);
HttpResponse getStylesheet(HttpServer* server, const HttpRequest& request);
HttpResponse getScript(HttpServer* server, const HttpRequest& request);
HttpResponse getLanguage(HttpServer* server, const HttpRequest& request);
HttpResponse sayHello(HttpServer* server, const HttpRequest& request);
HttpResponse redirect(HttpServer* server, const HttpRequest& request);

HttpResponse argumentTestInt(HttpServer* server, const HttpRequest& request);
HttpResponse argumentTestString(HttpServer* server, const HttpRequest& request);