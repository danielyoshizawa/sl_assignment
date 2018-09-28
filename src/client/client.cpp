#include "client.h"
#include <iostream>

namespace client
{
	client::~client()
	{

	}

	client::client()
	{

	}

	bool client::CreatePipe()
	{
		pipe = CreateFile(
			"\\\\.\\pipe\\sl_pipe",
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

		return isPipeValid();
	}

	int client::ClosePipe()
	{
		return CloseHandle(pipe);
	}

	int client::Read(void * buffer, DWORD numberOfBytesToRead, LPDWORD numberOfBytesRead, LPOVERLAPPED overlapped)
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

	int client::Write(void * buffer, DWORD numberOfBytesToRead, LPDWORD numberOfBytesRead, LPOVERLAPPED overlapped)
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
}