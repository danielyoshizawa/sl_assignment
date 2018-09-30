#include "server.h"
#include <iostream>
#include "console.h"
#include <sstream>
#include <strsafe.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

#include "message.h"

PIPEINST Pipe[INSTANCES];
HANDLE hEvents[INSTANCES];

namespace server
{
	Server::Server(const std::string & name) : name(name)
	{
		processor = std::make_shared<Processor>();
	}

	int Server::Start()
	{
		for (DWORD i = 0; i < INSTANCES; i++)
		{

			// Create an event object for this instance. 

			hEvents[i] = CreateEvent(
				NULL,    // default security attribute 
				TRUE,    // manual-reset event 
				TRUE,    // initial state = signaled 
				NULL);   // unnamed event object 

			if (hEvents[i] == NULL)
			{
				std::string message = "CreateEvent failed with " + GetLastError();
				console::print(name, message, console::SEVERITY::ERROR_S);
				return false;
			}
			else
			{
				std::stringstream msg;
				msg << "Event ";
				msg << i;
				msg << " created.";
				console::print(name, msg.str(), console::SEVERITY::NOTICE);
			}

			Pipe[i].oOverlap.hEvent = hEvents[i];

			Pipe[i].hPipeInst = CreateNamedPipe(
				lpszPipename,            // pipe name 
				PIPE_ACCESS_DUPLEX |     // read/write access 
				FILE_FLAG_OVERLAPPED,    // overlapped mode 
				PIPE_TYPE_MESSAGE |      // message-type pipe 
				PIPE_READMODE_MESSAGE |  // message-read mode 
				PIPE_WAIT,               // blocking mode 
				INSTANCES,               // number of instances 
				BUFSIZE * sizeof(TCHAR),   // output buffer size 
				BUFSIZE * sizeof(TCHAR),   // input buffer size 
				PIPE_TIMEOUT,            // client time-out 
				NULL);                   // default security attributes 

			if (Pipe[i].hPipeInst == INVALID_HANDLE_VALUE)
			{
				printf("CreateNamedPipe failed with %d.\n", GetLastError());
				return 0;
			}

			// Call the subroutine to connect to the new client

			Pipe[i].fPendingIO = ConnectToNewClient(
				Pipe[i].hPipeInst,
				&Pipe[i].oOverlap);

			Pipe[i].dwState = Pipe[i].fPendingIO ?
				CONNECTING_STATE : // still connecting 
				READING_STATE;     // ready to read 
		}

		return 1;
	}

	int Server::Run()
	{
		DWORD i, dwWait, cbRet, dwErr;
		BOOL fSuccess;

		while (1)
		{
			// Wait for the event object to be signaled, indicating 
			// completion of an overlapped read, write, or 
			// connect operation. 

			dwWait = WaitForMultipleObjects(
				INSTANCES,    // number of event objects 
				hEvents,      // array of event objects 
				FALSE,        // does not wait for all 
				INFINITE);    // waits indefinitely 

		  // dwWait shows which pipe completed the operation. 

			i = dwWait - WAIT_OBJECT_0;  // determines which pipe 
			if (i < 0 || i >(INSTANCES - 1))
			{
				printf("Index out of range.\n");
				return 0;
			}

			// Get the result if the operation was pending. 

			if (Pipe[i].fPendingIO)
			{
				fSuccess = GetOverlappedResult(
					Pipe[i].hPipeInst, // handle to pipe 
					&Pipe[i].oOverlap, // OVERLAPPED structure 
					&cbRet,            // bytes transferred 
					FALSE);            // do not wait 

				switch (Pipe[i].dwState)
				{
					// Pending connect operation 
				case CONNECTING_STATE:
					if (!fSuccess)
					{
						printf("Error %d.\n", GetLastError());
						return 0;
					}
					else
					{
						std::string message = "Client has connected";
						console::print(name, message, console::SEVERITY::NOTICE);
					}

					Pipe[i].dwState = READING_STATE;
					break;

					// Pending read operation 
				case READING_STATE:
					if (!fSuccess || cbRet == 0)
					{
						DisconnectAndReconnect(i);
						continue;
					}

					Pipe[i].cbRead = cbRet;
					Pipe[i].dwState = WRITING_STATE;
					break;

					// Pending write operation 
				case WRITING_STATE:
					if (!fSuccess || cbRet != Pipe[i].cbToWrite)
					{
						DisconnectAndReconnect(i);
						continue;
					}
					Pipe[i].dwState = READING_STATE;
					break;

				default:
				{
					printf("Invalid pipe state.\n");
					return 0;
				}
				}
			}

			// The pipe state determines which operation to do next. 

			switch (Pipe[i].dwState)
			{
				// READING_STATE: 
				// The pipe instance is connected to the client 
				// and is ready to read a request from the client. 

			case READING_STATE:
				fSuccess = ReadFile(
					Pipe[i].hPipeInst,
					Pipe[i].chRequest,
					BUFSIZE * sizeof(TCHAR),
					&Pipe[i].cbRead,
					&Pipe[i].oOverlap);

				// The read operation completed successfully. 

				if (fSuccess && Pipe[i].cbRead != 0)
				{
					Pipe[i].fPendingIO = FALSE;
					Pipe[i].dwState = WRITING_STATE;

					std::string message = "Read process completed";
					console::print(name, message, console::SEVERITY::NOTICE);

					continue;
				}

				// The read operation is still pending. 

				dwErr = GetLastError();
				if (!fSuccess && (dwErr == ERROR_IO_PENDING))
				{
					Pipe[i].fPendingIO = TRUE;
					continue;
				}

				// An error occurred; disconnect from the client. 

				DisconnectAndReconnect(i);
				break;

				// WRITING_STATE: 
				// The request was successfully read from the client. 
				// Get the reply data and write it to the client. 

			case WRITING_STATE:
				GetAnswerToRequest(&Pipe[i]);

				fSuccess = WriteFile(
					Pipe[i].hPipeInst,
					Pipe[i].chReply,
					Pipe[i].cbToWrite,
					&cbRet,
					&Pipe[i].oOverlap);

				// The write operation completed successfully. 

				if (fSuccess && cbRet == Pipe[i].cbToWrite)
				{
					Pipe[i].fPendingIO = FALSE;
					Pipe[i].dwState = READING_STATE;

					std::string message = "Write process completed";
					console::print(name, message, console::SEVERITY::NOTICE);

					continue;
				}

				// The write operation is still pending. 

				dwErr = GetLastError();
				if (!fSuccess && (dwErr == ERROR_IO_PENDING))
				{
					Pipe[i].fPendingIO = TRUE;
					continue;
				}

				// An error occurred; disconnect from the client. 

				DisconnectAndReconnect(i);
				break;

			default:
			{
				printf("Invalid pipe state.\n");
				return 0;
			}
			}
		}

		return 1;
	}

	VOID Server::DisconnectAndReconnect(DWORD i)
	{
		// Disconnect the pipe instance. 

		if (!DisconnectNamedPipe(Pipe[i].hPipeInst))
		{
			printf("DisconnectNamedPipe failed with %d.\n", GetLastError());
		}
		else
		{
			std::string message = "Client has disconnected";
			console::print(name, message, console::SEVERITY::NOTICE);
		}

		// Call a subroutine to connect to the new client. 

		Pipe[i].fPendingIO = ConnectToNewClient(
			Pipe[i].hPipeInst,
			&Pipe[i].oOverlap);

		Pipe[i].dwState = Pipe[i].fPendingIO ?
			CONNECTING_STATE : // still connecting 
			READING_STATE;     // ready to read 
	}

	BOOL Server::ConnectToNewClient(HANDLE hPipe, LPOVERLAPPED lpo)
	{
		BOOL fConnected, fPendingIO = FALSE;

		fConnected = ConnectNamedPipe(hPipe, lpo);

		if (fConnected)
		{
			std::string message = "ConnectNamedPipe failed with " + GetLastError();
			console::print(name, message, console::SEVERITY::ERROR_S);
			return 0;
		}
		
		switch (GetLastError())
		{
		case ERROR_IO_PENDING:
			fPendingIO = TRUE;
			break;
		case ERROR_PIPE_CONNECTED:
			if (SetEvent(lpo->hEvent))
			{
				break;
			}
		default:
		{
			std::string message = "ConnectedNamePipe failed with " + GetLastError();
			console::print(name, message, console::SEVERITY::ERROR_S);
			return 0;
		}
		}

		return fPendingIO;
	}

	VOID Server::GetAnswerToRequest(LPPIPEINST pipe)
	{
		std::string bufferOut = processor->Process(pipe->chRequest);
		StringCchCopy(pipe->chReply, BUFFERSIZE, TEXT(bufferOut.c_str()));
		pipe->cbToWrite = (lstrlen(pipe->chReply) + 1) * sizeof(TCHAR);
		
		memset(pipe->chRequest, 0, BUFFERSIZE);
	}

}