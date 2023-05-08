#pragma once

#include "HttpResponse.hpp"

#include <functional>
#include <vector>
#include <string>

using HttpArgs = std::vector<std::string>;

class HttpServer;
using HttpCallback = std::function<HttpResponse(HttpServer*, const HttpArgs&)>;