#pragma once
#ifndef CLIENT_H
#define CLIENT_H

#include "common.h"
#include <string>
#include "message.h"

namespace client
{
	class client
	{
	private:
		HANDLE pipe;
		std::string name;
	public:
		virtual ~client();
		client(const std::string & name);
		int Start();
		int ClosePipe();
		void PrintMessage(const std::string & message);
		void CreateCustom(const std::string & name, const std::string & message);
		void CustomsList();
		void RetrieveCustom(const std::string & name);
	private:
		int createPipe();
		bool isPipeValid();
		void sendSync(const message::message & messageToSend, message::message & messageReceived);
		void sendAscync(const message::message & message);
		int read(void * buffer, DWORD numberOfBytesToRead, LPDWORD numberOfBytesRead, LPOVERLAPPED overlapped);
		int write(void * buffer, DWORD numberOfBytesToRead, LPDWORD numberOfBytesRead, LPOVERLAPPED overlapped);
	};
};

#endif // CLIENT_H