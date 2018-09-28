#define NOMINMAX
#include <iostream>
#include <Windows.h>
#include "client.h"
#include <string>
#include "message.h"
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>
#include <sstream>
#include "custom.h"

void sendMessage(std::shared_ptr<client::client> client_sptr, std::stringstream & ss)
{
	DWORD numBytesWritten = 0;
	BOOL result = client_sptr->Write(
		(void *)ss.str().c_str(),
		(int)ss.tellp(),
		&numBytesWritten,
		NULL
	);

	if (result)
	{
		std::cout << "Number of bytes sent : " << numBytesWritten << std::endl;
	}
	else
	{
		std::cout << "Failed to send data :: " << GetLastError() << " - num - " << numBytesWritten << std::endl;
	}
}

int main(int argc, const char **argv)
{
	std::cout << "Connecting to server" << std::endl;

	std::shared_ptr<client::client> client_sptr = std::make_shared<client::client>();

	bool success = client_sptr->CreatePipe();

	if (!success)
	{
		std::cout << "Failed to connect to the server :: " << GetLastError() << std::endl;
		system(" pause");
		return 1;
	}

	std::cout << "Sending data" << std::endl;

	///////////////////////////////////////////////////////
	//
	// Creating objects on the server side
	//
	///////////////////////////////////////////////////////
	{
		message::message msg;
		msg.method = "createCustom";
		msg.params.push_back("nameCustom01");
		msg.params.push_back("test message");

		std::stringstream ss;
		{
			cereal::JSONOutputArchive oarchive(ss);
			oarchive(msg);
		}

		sendMessage(client_sptr, ss);
	}

	{
		message::message msg;
		msg.method = "createCustom";
		msg.params.push_back("nameCustom02");
		msg.params.push_back("test message");

		std::stringstream ss;
		{
			cereal::JSONOutputArchive oarchive(ss);
			oarchive(msg);
		}

		sendMessage(client_sptr, ss);
	}

	{
		message::message msg;
		msg.method = "createCustom";
		msg.params.push_back("nameCustom03");
		msg.params.push_back("test message");

		std::stringstream ss;
		{
			cereal::JSONOutputArchive oarchive(ss);
			oarchive(msg);
		}

		sendMessage(client_sptr, ss);
	}

	///////////////////////////////////////////////////////
	//
	// Retrieving custom object names
	//
	///////////////////////////////////////////////////////

	message::message msgNames;
	msgNames.method = "customsList";
	std::stringstream ssNames;
	std::string customObjectName = "";

	{
		cereal::JSONOutputArchive oarchive(ssNames);
		oarchive(msgNames);
	}

	sendMessage(client_sptr, ssNames);

	{
		char buffer[4096];
		DWORD numBytesRead = 0;
		BOOL result = client_sptr->Read(
			buffer,
			4095 * sizeof(char),
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
		
		try
		{
			std::stringstream ssNames;
			ssNames << buffer;

			cereal::JSONInputArchive iarchive(ssNames);

			message::message msg;
			iarchive(msg);

			customObjectName = msg.params.at(1);
		}
		catch (...)
		{
			std::cout << "Unable to convert object" << std::endl;
		}
		
	}
	///////////////////////////////////////////////////////
	//
	// Retrieving custom object from server
	//
	///////////////////////////////////////////////////////
	{
		message::message msgRetrieve;
		msgRetrieve.method = "retrieveCustom";
		msgRetrieve.params.push_back(customObjectName);
		std::stringstream ssRetrieve;

		{
			cereal::JSONOutputArchive oarchive(ssRetrieve);
			oarchive(msgRetrieve);
		}

		sendMessage(client_sptr, ssRetrieve);

		char buffer[4096];
		DWORD numBytesRead = 0;
		BOOL result = client_sptr->Read(
			buffer,
			4095 * sizeof(char),
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

		try
		{
			std::stringstream ssCustom;
			ssCustom << buffer;

			cereal::JSONInputArchive iarchive(ssCustom);

			std::shared_ptr<custom::custom> custom_sptr;
			iarchive(custom_sptr);

			custom_sptr->printInfo();
		}
		catch (...)
		{
			std::cout << "Unable to convert object" << std::endl;
		}
	}

	///////////////////////////////////////////////////////
	//
	// Closing server
	//
	///////////////////////////////////////////////////////

	message::message msgClose;
	msgClose.method = "closeServer";

	std::stringstream ssClose;
	{
		cereal::JSONOutputArchive oarchive(ssClose);
		oarchive(msgClose);
	}

	sendMessage(client_sptr, ssClose);

	client_sptr->ClosePipe();

	std::cout << "Exiting..." << std::endl;

	return 0;
}