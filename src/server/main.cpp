#include <iostream>
#include <Windows.h>
#include "server.h"

int main(int argc, const char **argv)
{
	std::cout << "Starting Named Pipe Server" << std::endl;

	std::shared_ptr<server::server> server_sptr = std::make_shared<server::server>();

	server_sptr->CreatePipe();

	std::cout << "Waiting client" << std::endl;

	BOOL result = server_sptr->ConnectClient();

	if (!result)
	{
		std::cout << "Failed to connect :: " << GetLastError() << std::endl;
		server_sptr->ClosePipe();
		system("pause");
		return 1;
	}

	std::cout << "Reading data from client" << std::endl;

	wchar_t buffer[1024];
	DWORD numBytesRead = 0;
	result = server_sptr->Read(
		buffer,
		1023 * sizeof(wchar_t),
		&numBytesRead,
		NULL
	);

	if (result)
	{
		buffer[numBytesRead / sizeof(wchar_t)] = '\0';
		std::cout << "Number of bytes read: " << numBytesRead << std::endl;
		std::wcout << "Message : " << buffer << std::endl;
	}
	else
	{
		std::cout << "Failed to read data from the pipe" << std::endl;
	}

	server_sptr->ClosePipe();

	std::cout << "Exiting..." << std::endl;

	return 0;
}