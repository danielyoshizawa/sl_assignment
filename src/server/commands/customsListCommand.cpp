#include "customsListCommand.h"
#include "console.h"
#include "commandsName.h"

namespace command
{
	CustomsListCommand::CustomsListCommand(const std::shared_ptr<custom::Manager> & customManager) : customManager(customManager)
	{
	}

	message::message CustomsListCommand::Execute(const std::vector<std::string> & params)
	{
		std::vector<std::string> customsList = customManager->GetListOfNames();

		message::message msg;
		msg.method = "success";
		msg.params.assign(customsList.begin(), customsList.end());

		console::print(CUSTOM_LIST_CMD, "Name of custom objects sent", console::SEVERITY::NOTICE);

		return msg;
	}
}