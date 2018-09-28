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
			"\\\\.\\pipe\\sl_pipe",
			PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
			PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE,
			1,
			4096,
			4096,
			200,
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

	int server::Read(void * buffer, DWORD numberOfBytesToRead, LPDWORD numberOfBytesRead, LPOVERLAPPED overlapped)
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

	int server::Write(void * buffer, DWORD numberOfBytesToRead, LPDWORD numberOfBytesRead, LPOVERLAPPED overlapped)
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