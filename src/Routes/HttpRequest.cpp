#include "Routes/HttpRequest.hpp"

HttpRequest::HttpRequest()
{

}

HttpRequest::HttpRequest(Args&& args) : mArgs(std::move(args))
{

}

HttpRequest::HttpRequest(Args&& args, FormData&& formData) : mArgs(std::move(args)), mFormData(std::move(formData))
{

}

HttpRequest::HttpRequest(const Args& args) : mArgs(args)
{

}

HttpRequest::HttpRequest(const Args& args, const FormData& formData) : mArgs(args), mFormData(formData)
{

}


HttpRequest::~HttpRequest()
{

}


const std::string& HttpRequest::getArgument(unsigned index) const
{
	if(index >= mArgs.size())
		return std::string("~ invalid access on argument request ~");

	return mArgs.at(index);
}

const std::string& HttpRequest::getFormAttribute(std::string_view key) const
{
	auto it = mFormData.find(std::string(key.begin(), key.end()));

	if(it == mFormData.end())
		return std::string("~ invalid access on form data request ~");

	return it->second;
}

const HttpRequest::FormData& HttpRequest::getFormData() const
{
	return mFormData;
}
