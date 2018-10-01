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
		void PrintMessage(const std::string & message, bool async = false);
		void CreateCustom(const std::string & name, const std::string & message, bool async = false);
		void CustomsList(bool async = false);
		void RetrieveCustom(const std::string & name, bool async = false);
	private:
		int createPipe();
		bool isPipeValid();
		void sendSync(const message::message & messageToSend, message::message & messageReceived);
		void sendAsync(const message::message & messageToSend, message::message & messageReceived);
		int read(void * buffer, DWORD numberOfBytesToRead, LPDWORD numberOfBytesRead, LPOVERLAPPED overlapped);
		int write(void * buffer, DWORD numberOfBytesToRead, LPDWORD numberOfBytesRead, LPOVERLAPPED overlapped);
	};
};

#endif // CLIENT_H