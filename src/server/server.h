#pragma once
#ifndef SERVER_H

#include "Windows.h"

namespace server
{
	class server
	{
	private:
		HANDLE pipe;
	public:
		virtual ~server();
		server();
		bool CreatePipe();
		int ConnectClient();
		int ClosePipe();
		int Write(LPVOID buffer, DWORD numberOfBytesToRead, LPDWORD numberOfBytesRead, LPOVERLAPPED overlapped);
	private:
		bool isPipeValid();
	};
};

#endif // SERVER_H