#pragma once

#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>
#include <memory>

#include "HttpController.hpp"
#include "HttpFilterManager.hpp"

/*	TODO:
 *	-> create an HttpFilterManager to handle filters // DONE
 *	-> add a priority system
 *	-> apply the sort on ArgumentContainer based on the priority
 *	-> create an HttpErrorManager to handle error callbacks
*/

using HttpMethod = http::verb;

class HttpRouter : public HttpFilterManager
{
	public:
		HttpRouter();
		virtual ~HttpRouter();
		
	public:

		using Ptr = std::shared_ptr<HttpRouter>;

	private:

		struct HttpEndpoint;
		using FixedContainer = std::unordered_map<std::string, HttpEndpoint>;
		using ArgumentContainer = std::vector<std::pair<std::string, HttpEndpoint>>;

		struct HttpEndpoint
		{
			FixedContainer fixedChildren;
			
			// vector é utilizado para possibilitar um sort
			ArgumentContainer argumentChildren;

			std::unordered_map<std::string, HttpCallback> callbacks; // http methods

			std::vector<HttpFilter> filters;

			int priority;
		};

		struct EndpointSearchData
		{
			std::stringstream pathStream;
			std::string buffer;

			HttpEndpoint* endpointPtr;
			
			std::vector<std::string> args;
		};

	public:

		void addEndpoint(const HttpMethod& method, const std::string& path, HttpCallback callback);
		HttpController getRequest(const http::request<http::dynamic_body>& request);
		HttpController getRequest(const HttpMethod& method, const std::string& path);

	private:
		HttpController getRequest(const std::string& method, const std::string& path);

		bool searchChildren(EndpointSearchData& data);
		bool lookInFixedContainer(EndpointSearchData& data);
		bool lookInArgumentContainer(EndpointSearchData& data);

		bool checkFilters(const std::string& data, const HttpEndpoint& endpoint);

		HttpEndpoint& gotoEndpoint(const std::string& path);

		HttpEndpoint* createIfDontExist(const std::string& value, FixedContainer& data);
		HttpEndpoint* createIfDontExist(const std::string& type, ArgumentContainer& data);
		void addFiltersToEndpoint(HttpEndpoint& endpoint, const std::string& type);

		// error callbacks
		static HttpResponse notFoundError(const std::vector<std::string>& args);

	private:

		HttpEndpoint mRootEndpoint;
};