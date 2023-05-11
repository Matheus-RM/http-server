#include "Base/HttpServer.hpp"

#include <functional>

#include <toml++/toml.h>
#include <inja/inja.hpp>

#include "Util/Message.hpp"

HttpServer::HttpServer()
	 : HttpRouter(), mContext(1), mAcceptor(mContext), mConnectionSocket(mContext)
{
	MESSAGE("BUILD MODE = \"", BUILD_MODE, "\"\n");
}

HttpServer::~HttpServer()
{
	MESSAGE("SERVER: the server is now closing\n");
}


void HttpServer::readConfigureFile(const std::string& path)
{
	auto config = toml::parse_file(path);

	mConfigure.name = config["default"]["name"].value_or("Theu-Server");
	mConfigure.ipAddress = config[BUILD_MODE]["address"].value_or("127.0.0.1");
	mConfigure.port = config[BUILD_MODE]["port"].value_or(8080);
	mConfigure.templatePath = config["default"]["template_dir"].value_or("templates/");
	mConfigure.errorTemplate = config["default"]["error_template"].value_or("error.html");
}

void HttpServer::configure()
{
	const auto endpoint = tcp::endpoint(asio::ip::make_address(mConfigure.ipAddress), mConfigure.port);
	mAcceptor = tcp::acceptor(mContext, endpoint);

	this->addDependency<inja::Environment>(mConfigure.templatePath);

	this->setErrorTemplateFile(mConfigure.errorTemplate);
}


void HttpServer::run()
{
	configure();

	MESSAGE("SERVER: server \"", mConfigure.name, "\" is now running\n");
	MESSAGE("\taddress=\"", mConfigure.ipAddress, "\", port=", mConfigure.port, "\n");

	acceptConnection();

	mContext.run();
}

void HttpServer::acceptConnection()
{
	if(mShouldQuit)
		return;

	mAcceptor.async_accept(mConnectionSocket,
				std::bind(&HttpServer::handleConnection, this, std::placeholders::_1)
			);
}

void HttpServer::handleConnection(const beast::error_code& error)
{
	if(!error)
    {
        auto connection = std::make_shared<HttpConnection>(std::move(mConnectionSocket), this);
        connection->start();
    }

	acceptConnection();
}

std::string HttpServer::getName() const
{
	return mConfigure.name;
}
