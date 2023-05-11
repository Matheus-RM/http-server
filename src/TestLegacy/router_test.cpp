#include <iostream>

#include "HttpRouter.hpp"

int main(int argc, char** argv)
{
	if(argc < 5)
	{
		std::cout << "Insufficient arguments.\n";
		return -1;
	}

	HttpRouter router;

	std::cout << "Adding controller...\n";

	router.addEndpoint(argv[1], argv[2], 
				[](const std::vector<std::string>& args) {
					std::ostringstream result;
					result << "Message = \"Hello World\"\nArguments:\n";

					for(const auto& arg : args)
					{
						result << "-> " << arg << "\n";
					}

					return result;
				});

	std::cout << "\n-> Getting controller...\n";

	auto controller = router.getRequest(argv[3], argv[4]);
	auto buffer = controller.execute();

	std::cout << "CONTROLLER RETURNED:\n" << buffer.str() << "END\n";

	return 0;
}