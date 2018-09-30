#include "server.h"
#include <iostream>

int main(int argc, const char **argv)
{
	try
	{
		std::unique_ptr<server::Server> server(new server::Server("SERVER"));
		server->Start();
		server->Run();
	}
	catch (std::exception & ex)
	{
		std::cout << "Exception : " << ex.what() << std::endl;
	}

	return 0;
}
