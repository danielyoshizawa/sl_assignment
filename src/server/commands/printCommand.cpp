#include "printCommand.h"
#include "console.h"
#include "commandsName.h"

namespace command
{
	message::message PrintCommand::Execute(const std::vector<std::string> & params)
	{
		message::message msg;

		if (params.empty())
		{
			console::print(PRINT_CMD, "Missing message to be printed", console::SEVERITY::WARNING);

			msg.method = "error";
			msg.params.push_back("401");
			msg.params.push_back("Message was missing.");
		}
		else
		{
			console::print(PRINT_CMD, params.at(0), console::SEVERITY::NOTICE);

			msg.method = "success";
			msg.params.push_back("200");
			msg.params.push_back("Message was printed.");
		}

		return msg;
	}
}