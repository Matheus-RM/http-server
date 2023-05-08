#pragma once

#include "HttpServer.hpp"
#include "HttpResponse.hpp"
#include "HttpCallback.hpp"
#include <vector>

std::string readFile(const std::string& path);

HttpResponse index(HttpServer* server, const HttpArgs& args);
HttpResponse getStylesheet(HttpServer* server, const HttpArgs& args);
HttpResponse getScript(HttpServer* server, const HttpArgs& args);
HttpResponse getLanguage(HttpServer* server, const HttpArgs& args);
HttpResponse sayHello(HttpServer* server, const HttpArgs& args);
HttpResponse redirect(HttpServer* server, const HttpArgs& args);
