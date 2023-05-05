#pragma once

#include "HttpResponse.hpp"
#include <vector>

std::string readFile(const std::string& path);

HttpResponse index(const std::vector<std::string>& args);
HttpResponse getStylesheet(const std::vector<std::string>& args);
HttpResponse getScript(const std::vector<std::string>& args);
HttpResponse getLanguage(const std::vector<std::string>& args);
