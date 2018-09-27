#include "server.h"
#include <iostream>

namespace server
{
	server::~server()
	{

	}

	server::server()
	{

	}

	bool server::CreatePipe()
	{
		pipe = CreateNamedPipe(
			"\\\\.\\pipe\\my_pipe",
			PIPE_ACCESS_DUPLEX,
			PIPE_TYPE_MESSAGE,
			1,
			0,
			0,
			0,
			NULL
		);

		return isPipeValid();
	}

	int server::ConnectClient()
	{
		if (!isPipeValid())
		{
			std::cout << "Invalid Pipe : Creation is required." << std::endl;
			return 1;
		}

		return ConnectNamedPipe(pipe, NULL);
	}

	int server::ClosePipe()
	{
		return CloseHandle(pipe);
	}

	int server::Read(LPVOID buffer, DWORD numberOfBytesToRead, LPDWORD numberOfBytesRead, LPOVERLAPPED overlapped)
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

	int server::Write(LPVOID buffer, DWORD numberOfBytesToRead, LPDWORD numberOfBytesRead, LPOVERLAPPED overlapped)
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

	bool server::isPipeValid()
	{
		return (pipe != NULL && pipe != INVALID_HANDLE_VALUE);
	}
}