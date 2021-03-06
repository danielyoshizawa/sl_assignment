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
				console::print(messageToSend.method, "Message received on server", console::SEVERITY::NOTICE);
				retryCount = 0;
			}
			else if (messageReceived.method == "error")
			{
				console::print(messageToSend.method, messageReceived.params.at(1), console::SEVERITY::WARNING);
				console::print(messageToSend.method, "Retring", console::SEVERITY::WARNING);
				retryCount--;
			}
			else
			{
				console::print(messageToSend.method, "Invalid anwser, aborting", console::SEVERITY::ERROR_S);
				retryCount = 0;
			}
		} while (retryCount);
	}

	void client::sendAsync(const message::message & messageToSend, message::message & messageReceived)
	{
		OVERLAPPED osWrite, osRead;
		DWORD numBytesWritten = 0;
		int retryCount = 3;
		memset(&osWrite, 0, sizeof(osWrite));
		memset(&osRead, 0, sizeof(osRead));
		osWrite.hEvent = CreateEvent(
							NULL,    // default security attribute 
							TRUE,    // manual-reset event 
							TRUE,    // initial state = signaled 
							NULL);   // unnamed event object 

		osRead.hEvent = CreateEvent(
							NULL,    // default security attribute 
							TRUE,    // manual-reset event 
							TRUE,    // initial state = signaled 
							NULL);   // unnamed event object 

		if (osWrite.hEvent == NULL || osRead.hEvent == NULL)
		{
			std::stringstream ss;
			ss << "CreateEvent failed with ";
			ss << GetLastError();
			console::print(name, ss.str(), console::SEVERITY::ERROR_S);
			return;
		}

		std::stringstream ssOut, ssIn;
		{
			cereal::JSONOutputArchive oarchive(ssOut);
			oarchive(messageToSend);
		}

		int err = 0;

		bool success = write(
							(void *)ssOut.str().c_str(),
							(int)ssOut.tellp(),
							NULL,
							&osWrite
						);
		err = GetLastError();

		if (err == ERROR_BROKEN_PIPE)
		{
			console::print(name, "Broken pipe", console::SEVERITY::ERROR_S);
			return;
		}

		if (!success && err == ERROR_IO_PENDING)
		{
			if (WaitForSingleObject(osWrite.hEvent, INFINITE) == WAIT_OBJECT_0)
			{
				GetOverlappedResult(pipe, &osWrite, &numBytesWritten, TRUE);
				console::print(name, "Async write operation finished", console::SEVERITY::NOTICE);
			}
			else
			{
				CancelIo(pipe);
				console::print(name, "Write was canceled due problems", console::SEVERITY::WARNING);
				return;
			}
		}
		else if (success)
		{
			console::print(name, "Async write operation finished - no wait", console::SEVERITY::NOTICE);
		}

		char buffer[BUFFERSIZE];

		success = read(
						buffer,
						BUFFERSIZE * sizeof(char),
						NULL,
						&osRead
					);

		err = GetLastError();

		if (err == ERROR_BROKEN_PIPE)
		{
			console::print(name, "Broken pipe", console::SEVERITY::ERROR_S);
			return;
		}
		
		if (!success && err == ERROR_IO_PENDING)
		{
			if (WaitForSingleObject(osRead.hEvent, INFINITE) == WAIT_OBJECT_0)
			{
				GetOverlappedResult(pipe, &osRead, &numBytesWritten, TRUE);
				console::print(name, "Async read operation finished", console::SEVERITY::NOTICE);
			}
			else
			{
				CancelIo(pipe);
				console::print(name, "Read was canceled due problems", console::SEVERITY::WARNING);
				return;
			}
		}
		else if (success)
		{
			console::print(name, "Async read operation finished - no wait", console::SEVERITY::NOTICE);
		}

		{
			ssIn << buffer;

			cereal::JSONInputArchive iarchive(ssIn);
			iarchive(messageReceived);
		}
	}

	void client::PrintMessage(const std::string & message, bool async)
	{
		message::message msgOut, msgIn;
		msgOut.method = PRINT_CMD;
		msgOut.params.push_back(message);

		async ? sendAsync(msgOut, msgIn) : sendSync(msgOut, msgIn);
	}

	void client::CreateCustom(const std::string & name, const std::string & message, bool async)
	{
		message::message msgOut, msgIn;
		msgOut.method = CREATE_CUSTOM_CMD;
		msgOut.params.push_back(name);
		msgOut.params.push_back(message);

		async ? sendAsync(msgOut, msgIn) : sendSync(msgOut, msgIn);
	}

	void client::CustomsList(bool async)
	{
		message::message msgOut, msgIn;
		msgOut.method = CUSTOM_LIST_CMD;
		
		async ? sendAsync(msgOut, msgIn) : sendSync(msgOut, msgIn);

		for (std::string name : msgIn.params)
		{
			console::print(CUSTOM_LIST_CMD, name, console::SEVERITY::NOTICE);
		}
	}

	void client::RetrieveCustom(const std::string & name, bool async)
	{
		message::message msgOut, msgIn;
		msgOut.method = RETRIEVE_CUSTOM_CMD;
		msgOut.params.push_back(name);

		async ? sendAsync(msgOut, msgIn) : sendSync(msgOut, msgIn);

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