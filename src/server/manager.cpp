#include "manager.h"
#include <iostream>
#include <sstream>
#include "message.h"
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>

namespace server
{
	manager::~manager()
	{

	}

	manager::manager() : running(true)
	{
		server_sptr = std::make_shared<server>();
	}

	void manager::start()
	{
		std::cout << "Starting Named Pipe Server" << std::endl;
		server_sptr->CreatePipe();
		BOOL result = server_sptr->ConnectClient();
		
		if (!result)
		{
			std::cout << "Failed to connect :: " << GetLastError() << std::endl;
			server_sptr->ClosePipe();
			system("pause");
			return;
		}

		std::cout << "Reading data from client" << std::endl;
		while (running)
		{
			char buffer[4096];
			DWORD numBytesRead = 0;
			result = server_sptr->Read(
				buffer,
				4096 * sizeof(char),
				&numBytesRead,
				NULL
			);

			if (result)
			{
				buffer[numBytesRead / sizeof(char)] = '\0';
				std::cout << "Number of bytes read: " << numBytesRead << std::endl;
				std::cout << "Message : " << buffer << std::endl;
			}
			else
			{
				std::cout << "Failed to read data from the pipe" << std::endl;
			}

			{
				std::stringstream ss;
				ss << buffer;

				cereal::JSONInputArchive iarchive(ss);

				message::message msg;
				iarchive(msg);
				process(msg);
				msgVector.push_back(msg);
			}
		}

		server_sptr->ClosePipe();

		for (custom_pair_type custom : customsMap)
		{
			custom.second->printInfo();
		}

		std::cout << "Exiting..." << std::endl;

	}

	/*
		Method list :
					- printMessage
						- message
					- createCustom
						- name
						- message
					- retrieveCustom
						- name
					- customsList
					- closeServer
	*/
	void manager::process(const message::message & msg)
	{
		std::string method = msg.method;

		if (method.compare("printMessage") == 0)
		{
			std::cout << "Message received :: " << std::endl;
			std::cout << msg.params.at(0)<< std::endl;
		}
		else if (method.compare("createCustom") == 0)
		{
			std::cout << "Creating custom object" << std::endl;
			std::string name = msg.params.at(0);
			std::string message = msg.params.at(1);
			createCustom(name, message);
		}
		else if (method.compare("retrieveCustom") == 0)
		{
			std::cout << "Sending custom object" << std::endl;
			std::string name = msg.params.at(0);
			std::shared_ptr<custom::custom> tmpCustom = customsMap.at(name);
			
			if (tmpCustom.get())
			{
				std::stringstream ss;
				{
					cereal::JSONOutputArchive oarchive(ss);
					oarchive(tmpCustom);

				}

				DWORD numBytesWritten = 0;
				BOOL result = server_sptr->Write(
					(void *)ss.str().c_str(),
					(int)ss.tellp(),
					&numBytesWritten,
					NULL
				);

				std::cout << "Custom Object sent : " << numBytesWritten << std::endl;
			}
			else 
			{
				std::cout << "Couldn't find custom object" << std::endl;
			}
		}
		else if (method.compare("customsList") == 0)
		{
			std::cout << "Sending name of custom objects " << std::endl;
			message::message msg;
			msg.method = "customsList";
			for (auto custom : customsMap)
			{
				msg.params.push_back(custom.first);
			}

			std::stringstream ss;
			{
				cereal::JSONOutputArchive oarchive(ss);
				oarchive(msg);

			}

			DWORD numBytesWritten = 0;
			BOOL result = server_sptr->Write(
				(void *)ss.str().c_str(),
				(int)ss.tellp(),
				&numBytesWritten,
				NULL
			);
		}
		else if (method.compare("closeServer") == 0)
		{
			running = false;
		}
		else
		{
			std::cout << "Method not found!" << std::endl;
		}
	}

	void manager::createCustom(const std::string & name, const std::string & message)
	{
		customsMap.insert(custom_pair_type(name, std::make_shared<custom::custom>(name, message)));
	}
}