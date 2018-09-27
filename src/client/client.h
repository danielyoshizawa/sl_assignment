#pragma once
#ifndef CLIENT_H
#define CLIENT_H

#include <Windows.h>

namespace client
{
	class client
	{
	private:
		HANDLE pipe;
	public:
		virtual ~client();
		client();
		bool CreatePipe();
		int ClosePipe();
		int Read(LPVOID buffer, DWORD numberOfBytesToRead, LPDWORD numberOfBytesRead, LPOVERLAPPED overlapped);
		int Write(LPVOID buffer, DWORD numberOfBytesToRead, LPDWORD numberOfBytesRead, LPOVERLAPPED overlapped);
	private:
		bool isPipeValid();
	};
};

#endif // CLIENT_H