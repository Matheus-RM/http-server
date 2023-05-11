#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

class HttpRequest
{
	public:
		using Args = std::vector<std::string>;
		using FormData = std::unordered_map<std::string, std::string>;

	public:
		HttpRequest();

		HttpRequest(Args&& args);
		HttpRequest(Args&& args, FormData&& formData);

		HttpRequest(const Args& args);
		HttpRequest(const Args& args, const FormData& formData);

		~HttpRequest();

	public:
		const std::string& getArgument(unsigned index) const;
		const std::string& getFormAttribute(std::string_view key) const;
		const FormData& getFormData() const;

	private:
		Args mArgs;
		FormData mFormData;
};