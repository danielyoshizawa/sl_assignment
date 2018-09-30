#pragma once
#ifndef SERVER_H
#define SERVER_H

#include <string>
#include "common.h"
#include "processor.h"

namespace server
{
	class Server
	{
	private:
		std::string name;
		std::shared_ptr<Processor> processor;
	public:
		virtual ~Server() {};
		Server(const std::string & name);
		int Start();
		int Run();
	private:
		VOID DisconnectAndReconnect(DWORD);
		BOOL ConnectToNewClient(HANDLE, LPOVERLAPPED);
		VOID GetAnswerToRequest(LPPIPEINST);
	};
};

#endif // SERVER_H
