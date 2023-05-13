#include "Util/HttpUtil.hpp"

#include <sstream>

#include "Util/Message.hpp"

std::string HttpUtil::decodeUrl(std::string_view request)
{
    std::string result;
    result.reserve(request.size());
    
    for(size_t i = 0; i < request.size(); i++)
    {
        const auto buffer = request[i];
        
        if (buffer == '+')
        {
            result.push_back(' ');
            continue;
        }
        
        if(buffer == '%' && (i + 2) < request.size())
        {
            const auto&& hexCode = request.substr(i+1, 2);
            const auto&& utfChar = static_cast<char>(std::stol(std::string(hexCode), nullptr, 16));
            result.push_back(std::move(utfChar));
            
            i += 2;
            continue;
        }
        
        result.push_back(buffer);
    }
    
    return result;
}

HttpRequest::FormData HttpUtil::extractFormData(std::string_view formText)
{
	HttpRequest::FormData result;

	std::stringstream buffer;
	buffer << HttpUtil::decodeUrl(formText);

	std::string expression;

	while(std::getline(buffer, expression, '&'))
    {
        const auto pos = expression.find('=');
        const auto&& key = expression.substr(0, pos);
        const auto&& value = expression.substr(pos+1, expression.size() - pos);

        MESSAGE("\t==> Found key ", std::quoted(key), " with value ", std::quoted(value), "\n");
        
        result.emplace(std::move(key), std::move(value));
    }

    return result;
}