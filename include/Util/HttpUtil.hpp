#pragma once

#include <string>
#include <string_view>

#include "Routes/HttpRequest.hpp"

namespace HttpUtil
{
	std::string decodeUrl(std::string_view request);
	HttpRequest::FormData extractFormData(std::string_view formText);
}