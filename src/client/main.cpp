#include "client.h"

int main(int argc, TCHAR  **argv)
{
	std::shared_ptr<client::client> client_sptr = std::make_shared<client::client>("CLIENT 001");

	bool success = client_sptr->Start();
	client_sptr->CustomsList();
	client_sptr->PrintMessage("Test Message");
	client_sptr->CreateCustom("nameCustom01", "object description for custom 01");
	client_sptr->CreateCustom("nameCustom02", "object description for custom 02");
	client_sptr->CreateCustom("nameCustom03", "object description for custom 03");
	client_sptr->PrintMessage("Another Message");
	
	client_sptr->CustomsList();
	client_sptr->CreateCustom("nameCustom04", "object description for custom 04");
	client_sptr->CreateCustom("nameCustom05", "object description for custom 05");
	client_sptr->CreateCustom("nameCustom06", "object description for custom 06");
	client_sptr->PrintMessage("Another Message");
	
	client_sptr->CustomsList();
	client_sptr->CreateCustom("nameCustom07", "object description for custom 07");
	client_sptr->CreateCustom("nameCustom08", "object description for custom 08");
	client_sptr->CreateCustom("nameCustom09", "object description for custom 09");
	client_sptr->PrintMessage("Another Message");
	
	client_sptr->CustomsList();
	client_sptr->RetrieveCustom("nameCustom01");
	client_sptr->RetrieveCustom("nameCustom04");
	client_sptr->RetrieveCustom("nameCustom07");

	client_sptr->ClosePipe();

	return 0;
}
