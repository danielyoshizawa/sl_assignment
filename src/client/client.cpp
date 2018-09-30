#include "client.h"
#include <iostream>
#include "common.h"
#include "console.h"
#include "commandsName.h"
#include "message.h"
#include "custom.h"

#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>

namespace client
{
	client::~client()
	{

	}

	client::client(const std::string & name) : name(name)
	{

	}

	int client::Start()
	{
		return createPipe();
	}

	int client::createPipe()
	{
		console::print(name, "Connecting to server", console::SEVERITY::NOTICE);

		pipe = CreateFile(
			lpszPipename,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_FLAG_OVERLAPPED,
			NULL
		);

		if (!isPipeValid())
		{
			std::string error_msg = "Failed to connect to the server :: " + GetLastError();
			console::print(name, error_msg, console::SEVERITY::ERROR_S);
			return false;
		}

		return true;
	}

	int client::ClosePipe()
	{
		console::print(name, "Closing client", console::SEVERITY::NOTICE);
		return CloseHandle(pipe);
	}

	int client::read(void * buffer, DWORD numberOfBytesToRead, LPDWORD numberOfBytesRead, LPOVERLAPPED overlapped)
	{
		if (!isPipeValid())
		{
			std::cout << "Invalid Pipe : Creation is required." << std::endl;
			return 1;
		}

		return ReadFile(
			pipe,
			buffer,
			numberOfBytesToRead,
			numberOfBytesRead,
			overlapped
		);
	}

	int client::write(void * buffer, DWORD numberOfBytesToRead, LPDWORD numberOfBytesRead, LPOVERLAPPED overlapped)
	{
		if (!isPipeValid())
		{
			std::cout << "Invalid Pipe : Creation is required." << std::endl;
			return 1;
		}

		return WriteFile(
			pipe,
			buffer,
			numberOfBytesToRead,
			numberOfBytesRead,
			overlapped
		);
	}

	bool client::isPipeValid()
	{
		return (pipe != NULL && pipe != INVALID_HANDLE_VALUE);
	}

	void client::sendSync(const message::message & messageToSend, message::message & messageReceived)
	{
		int retryCount = 3;
		DWORD numBytesWritten = 0;
		do
		{
			std::stringstream ssOut, ssIn;
			{
				cereal::JSONOutputArchive oarchive(ssOut);
				oarchive(messageToSend);
			}

			write(
				(void *)ssOut.str().c_str(),
				(int)ssOut.tellp(),
				&numBytesWritten,
				NULL
			);

			{
				std::stringstream msgConsole;
				msgConsole << messageToSend.method;
				msgConsole << " was sent";
				console::print(name, msgConsole.str(), console::SEVERITY::NOTICE);
			}

			char buffer[BUFFERSIZE];

			read(
				buffer,
				BUFFERSIZE * sizeof(char),
				&numBytesWritten,
				NULL
			);

			{
				ssIn << buffer;

				cereal::JSONInputArchive iarchive(ssIn);
				iarchive(messageReceived);
			}

			if (messageReceived.method == "success")
			{
				console::print(name, "Message received on server", console::SEVERITY::NOTICE);
				retryCount = 0;
			}
			else if (messageReceived.method == "error")
			{
				console::print(name, messageReceived.params.at(1), console::SEVERITY::WARNING);
				console::print(name, "Retring", console::SEVERITY::WARNING);
				retryCount--;
			}
			else
			{
				console::print(name, "Invalid anwser, aborting", console::SEVERITY::ERROR_S);
				retryCount = 0;
			}
		} while (retryCount);
	}

	void client::PrintMessage(const std::string & message)
	{
		message::message msgOut, msgIn;
		msgOut.method = PRINT_CMD;
		msgOut.params.push_back(message);

		sendSync(msgOut, msgIn);
	}

	void client::CreateCustom(const std::string & name, const std::string & message)
	{
		message::message msgOut, msgIn;
		msgOut.method = CREATE_CUSTOM_CMD;
		msgOut.params.push_back(name);
		msgOut.params.push_back(message);

		sendSync(msgOut, msgIn);
	}

	void client::CustomsList()
	{
		message::message msgOut, msgIn;
		msgOut.method = CUSTOM_LIST_CMD;
		
		sendSync(msgOut, msgIn);

		for (std::string name : msgIn.params)
		{
			console::print(CUSTOM_LIST_CMD, name, console::SEVERITY::NOTICE);
		}
	}

	void client::RetrieveCustom(const std::string & name)
	{
		message::message msgOut, msgIn;
		msgOut.method = RETRIEVE_CUSTOM_CMD;
		msgOut.params.push_back(name);

		sendSync(msgOut, msgIn);
		if (msgIn.params.size() == 3)
		{
			std::shared_ptr<custom::custom> customObj;
			{
				std::stringstream ss;
				ss << msgIn.params.at(2);

				cereal::JSONInputArchive iarchive(ss);
				iarchive(customObj);
			}

			console::print(customObj->getName(), customObj->getMessage(), console::SEVERITY::NOTICE);

			customObj->printInfo();
		}
	}
}