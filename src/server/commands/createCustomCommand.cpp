#include "createCustomCommand.h"
#include "console.h"
#include "commandsName.h"

namespace command
{
	CreateCustomCommand::CreateCustomCommand(const std::shared_ptr<custom::Manager> & customManager) : customManager(customManager)
	{

	}

	message::message CreateCustomCommand::Execute(const std::vector<std::string> & params)
	{
		message::message msg;

		if (params.size() != 2)
		{
			console::print(CREATE_CUSTOM_CMD, "Missing arguments", console::SEVERITY::WARNING);

			msg.method = "error";
			msg.params.push_back("401");
			msg.params.push_back("Arguments were missing.");
		}
		else 
		{
			customManager->CreateCustom(params.at(0), params.at(1));

			console::print(CREATE_CUSTOM_CMD, "Custom object created", console::SEVERITY::NOTICE);

			msg.method = "success";
			msg.params.push_back("200");
			msg.params.push_back("Object created");
		}

		return msg;
	}
}