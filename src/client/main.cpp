#include <iostream>
#include <Windows.h>
#include "client.h"
#include <string>

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

	std::cout << "Sending data" << std::endl;
	const wchar_t *data = L"StreamLabs";
	DWORD numBytesWritten = 0;

	BOOL result = client_sptr->Write(
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

	client_sptr->ClosePipe();

	std::cout << "Exiting..." << std::endl;

	return 0;
}