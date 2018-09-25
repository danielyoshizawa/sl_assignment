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

	std::cout << "Sending data" << std::endl;
	const wchar_t *data = L"*** Hello pipe ***";
	DWORD numBytesWritten = 0;

	result = server_sptr->Write(
								(LPVOID)data,
								wcslen(data),
								&numBytesWritten,
								NULL
								);

	if (result)
	{
		std::cout << "Number of bytes sent : " << numBytesWritten << std::endl;
	}
	else
	{
		std::cout << "Failed to send data :: " << GetLastError() << std::endl;
	}

	server_sptr->ClosePipe();

	std::cout << "Exiting..." << std::endl;
	system("pause");
	return 0;
}