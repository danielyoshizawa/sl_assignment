#include <iostream>
#include <Windows.h>

int main(int argc, const char **argv)
{
	std::cout << "Starting Named Pipe Server" << std::endl;

	HANDLE pipe = CreateNamedPipe(
		"\\\\.\\pipe\\my_pipe",
		PIPE_ACCESS_OUTBOUND,
		PIPE_TYPE_BYTE,
		1,
		0,
		0,
		0,
		NULL
	);

	if (pipe == NULL || pipe == INVALID_HANDLE_VALUE)
	{
		std::cout << "Failed to create pipe :: " << GetLastError() << std::endl;
		system("pause");
		return 1;
	}

	std::cout << "Waiting client" << std::endl;

	BOOL result = ConnectNamedPipe(pipe, NULL);
	if (!result)
	{
		std::cout << "Failed to connect :: " << GetLastError() << std::endl;
		CloseHandle(pipe);
		system("pause");
		return 1;
	}

	std::cout << "Sending data" << std::endl;
	const wchar_t *data = L"*** Hello pipe ***";
	DWORD numBytesWritten = 0;
	result = WriteFile(
		pipe,
		data,
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

	CloseHandle(pipe);

	std::cout << "Exiting..." << std::endl;
	system("pause");
	return 0;
}