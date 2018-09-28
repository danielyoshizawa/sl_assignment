#define NOMINMAX
#include "manager.h"

int main(int argc, const char **argv)
{
	try
	{
		std::unique_ptr<server::manager> manager(new server::manager());
		manager->start();
	}
	catch (...)
	{
		std::cout << "Some exception happened, somewhere, somehow" << std::endl;
	}

	return 0;
}