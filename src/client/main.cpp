#include <iostream>
#include <Windows.h>
#include "client.h"

int main(int argc, const char **argv)
{
	std::cout << "Connecting to server" << std::endl;

	std::shared_ptr<client::client> client_sptr = std::make_shared<client::client>();
	
	bool success = client_sptr->CreatePipe();

	if (!success)
	{
		std::cout << "Failed to connect to the server :: " << GetLastError() << std::endl;
		system(" pause");
		return 1;
	}

	std::cout << "Reading data from server" << std::endl;

	wchar_t buffer[128];
	DWORD numBytesRead = 0;
	BOOL result = client_sptr->Read(
		buffer,
		127 * sizeof(wchar_t),
		&numBytesRead,
		NULL
	);

	if (result)
	{
		buffer[numBytesRead / sizeof(wchar_t)] = '\0';
		std::cout << "Number of bytes read: " << numBytesRead << std::endl;
		std::cout << "Message : " << buffer << std::endl;
	}
	else
	{
		std::cout << "Failed to read data from the pipe" << std::endl;
	}

	client_sptr->ClosePipe();

	std::cout << "Exiting..." << std::endl;
	system("pause");

	return 0;
}