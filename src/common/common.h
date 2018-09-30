#pragma once
#ifndef COMMON_H
#define COMMON_H

#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>

#define INSTANCES 4 
#define PIPE_TIMEOUT 5000
#define BUFSIZE 4096
#define BUFFERSIZE 4096

#define CONNECTING_STATE 0 
#define READING_STATE 1 
#define WRITING_STATE 2 

#define lpszPipename "\\\\.\\pipe\\mynamedpipe"

typedef struct
{
	OVERLAPPED oOverlap;
	HANDLE hPipeInst;
	TCHAR chRequest[BUFSIZE];
	DWORD cbRead;
	TCHAR chReply[BUFSIZE];
	DWORD cbToWrite;
	DWORD dwState;
	BOOL fPendingIO;
} PIPEINST, *LPPIPEINST;

#endif // COMMON_H