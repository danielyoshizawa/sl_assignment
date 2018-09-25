#include <iostream>
#include <Windows.h>


int main(int argc, const char **argv)
{
	std::cout << "Connecting to server" << std::endl;

	HANDLE pipe = CreateFile(
		"\\\\.\\pipe\\my_pipe",
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (pipe == INVALID_HANDLE_VALUE)
	{
		std::cout << "Failed to connect to the server :: " << GetLastError() << std::endl;
		system(" pause");
		return 1;
	}

	std::cout << "Reading data from server" << std::endl;

	wchar_t buffer[128];
	DWORD numBytesRead = 0;
	BOOL result = ReadFile(
		pipe,
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

	CloseHandle(pipe);

	std::cout << "Exiting..." << std::endl;
	system("pause");

	return 0;
}