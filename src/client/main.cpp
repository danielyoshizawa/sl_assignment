#include "client.h"
#include <Windows.h>
#include <process.h>
#include <iostream>
#include <string>

unsigned int __stdcall clientThreadA(void* data)
{
	std::shared_ptr<client::client> client_sptr = std::make_shared<client::client>("CLIENT 001");

	bool success = client_sptr->Start();

	if (success)
	{
		for (int i = 0; i < 10; ++i)
		{
			std::string objName;
			objName = "nameCustom1" + std::to_string(i);
			client_sptr->CustomsList();
			client_sptr->PrintMessage("Test Message");
			client_sptr->CreateCustom(objName, "object description for custom 01");
			
			objName = "nameCustom11" + std::to_string(i);
			client_sptr->CreateCustom(objName, "object description for custom 02");

			objName = "nameCustom111" + std::to_string(i);
			client_sptr->CreateCustom(objName, "object description for custom 03");
			client_sptr->PrintMessage("Another Message");
			
			client_sptr->CustomsList();
			client_sptr->RetrieveCustom("nameCustom31");
			client_sptr->RetrieveCustom("nameCustom444");
			client_sptr->RetrieveCustom("nameCustom117");

			Sleep(200);
		}
	}

	client_sptr->ClosePipe();

	return 0;
}

unsigned int __stdcall clientThreadB(void* data)
{
	std::shared_ptr<client::client> client_sptr = std::make_shared<client::client>("CLIENT 002");

	bool success = client_sptr->Start();

	if (success)
	{
		for (int i = 0; i < 10; ++i)
		{
			std::string objName;
			objName = "nameCustom2" + std::to_string(i);
			client_sptr->CustomsList(true);

			client_sptr->PrintMessage("Test Message", true);
			client_sptr->CreateCustom(objName, "object description for custom 01", true);

			objName = "nameCustom22" + std::to_string(i);
			client_sptr->CreateCustom(objName, "object description for custom 02", true);

			objName = "nameCustom222" + std::to_string(i);
			client_sptr->CreateCustom(objName, "object description for custom 03", true);
			client_sptr->PrintMessage("Another Message", true);

			client_sptr->CustomsList(true);
			client_sptr->RetrieveCustom("nameCustom41", true);
			client_sptr->RetrieveCustom("nameCustom334", true);
			client_sptr->RetrieveCustom("nameCustom227", true);

			Sleep(200);
		}
	}

	client_sptr->ClosePipe();

	return 0;
}

unsigned int __stdcall clientThreadC(void* data)
{
	std::shared_ptr<client::client> client_sptr = std::make_shared<client::client>("CLIENT 003");

	bool success = client_sptr->Start();

	if (success)
	{
		for (int i = 0; i < 10; ++i)
		{
			std::string objName;
			objName = "nameCustom3" + std::to_string(i);
			client_sptr->CustomsList();

			client_sptr->PrintMessage("Test Message");
			client_sptr->CreateCustom(objName, "object description for custom 01");

			objName = "nameCustom33" + std::to_string(i);
			client_sptr->CreateCustom(objName, "object description for custom 02");

			objName = "nameCustom333" + std::to_string(i);
			client_sptr->CreateCustom(objName, "object description for custom 03");
			client_sptr->PrintMessage("Another Message");

			client_sptr->CustomsList();
			client_sptr->RetrieveCustom("nameCustom21");
			client_sptr->RetrieveCustom("nameCustom114");
			client_sptr->RetrieveCustom("nameCustom337");

			Sleep(200);
		}
	}

	client_sptr->ClosePipe();

	return 0;
}

unsigned int __stdcall clientThreadD(void* data)
{
	std::shared_ptr<client::client> client_sptr = std::make_shared<client::client>("CLIENT 004");

	bool success = client_sptr->Start();

	if (success)
	{
		for (int i = 0; i < 10; ++i)
		{
			std::string objName;
			objName = "nameCustom4" + std::to_string(i);
			client_sptr->CustomsList(true);

			client_sptr->PrintMessage("Test Message", true);
			client_sptr->CreateCustom(objName, "object description for custom 01", true);

			objName = "nameCustom44" + std::to_string(i);
			client_sptr->CreateCustom(objName, "object description for custom 02", true);

			objName = "nameCustom444" + std::to_string(i);
			client_sptr->CreateCustom(objName, "object description for custom 03", true);
			client_sptr->PrintMessage("Another Message", true);

			client_sptr->CustomsList(true);
			client_sptr->RetrieveCustom("nameCustom21", true);
			client_sptr->RetrieveCustom("nameCustom114", true);
			client_sptr->RetrieveCustom("nameCustom4447", true);

			Sleep(200);
		}
	}

	client_sptr->ClosePipe();

	return 0;
}

int main(int argc, TCHAR  **argv)
{
	bool multi = false;

	if (argc == 2)
	{
		std::string arg_str(argv[1]);
		multi = arg_str.compare("--multi") == 0;
	}

	if (multi)
	{
		HANDLE clientsHandle[4];

		clientsHandle[0] = (HANDLE)_beginthreadex(0, 0, &clientThreadA, 0, 0, 0);
		clientsHandle[1] = (HANDLE)_beginthreadex(0, 0, &clientThreadB, 0, 0, 0);
		clientsHandle[2] = (HANDLE)_beginthreadex(0, 0, &clientThreadC, 0, 0, 0);
		clientsHandle[3] = (HANDLE)_beginthreadex(0, 0, &clientThreadD, 0, 0, 0);

		WaitForMultipleObjects(4, clientsHandle, true, INFINITE);

		CloseHandle(clientsHandle[0]);
		CloseHandle(clientsHandle[1]);
		CloseHandle(clientsHandle[2]);
		CloseHandle(clientsHandle[3]);

		getchar();
	}
	else
	{
		std::shared_ptr<client::client> client_sptr = std::make_shared<client::client>("CLIENT");

		bool success = client_sptr->Start();

		if (success)
		{
			client_sptr->CustomsList();
			client_sptr->PrintMessage("Test Message");
			client_sptr->CreateCustom("namedObject01", "object description for custom 01", true);
			client_sptr->CreateCustom("namedObject02", "object description for custom 02");
			client_sptr->CreateCustom("namedObject03", "object description for custom 03", true);
			client_sptr->PrintMessage("Another Message", true);

			client_sptr->CustomsList(true);
			client_sptr->RetrieveCustom("namedObject01", true);
			client_sptr->RetrieveCustom("namedObject02");
			client_sptr->RetrieveCustom("namedObject04", true);
		}

		client_sptr->ClosePipe();
	}
	return 0;
}
