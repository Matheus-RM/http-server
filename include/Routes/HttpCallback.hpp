#pragma once

#include "Routes/HttpRequest.hpp"
#include "Routes/HttpResponse.hpp"

#include <functional>

class HttpServer;
using HttpCallback = std::function<HttpResponse(HttpServer*, const HttpRequest&)>;