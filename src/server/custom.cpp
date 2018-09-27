#include "custom.h"
#include <iostream>

namespace server
{
	custom::~custom()
	{

	}

	custom::custom()
	{

	}
	
	void custom::printInfo()
	{
		std::cout << " :: Info about custom object :: " << std::endl;
		std::cout << " :: Name : " << name << " :: " << std::endl;
		std::cout << " :: Message : " << message << " :: " << std::endl;
	}

	void custom::setName(const std::string & _name)
	{
		name = _name;
	}

	void custom::setMessage(const std::string & _message)
	{
		message = _message;
	}

	std::string custom::getName() const
	{
		return name;
	}

	std::string custom::getMessage() const
	{
		return message;
	}
}