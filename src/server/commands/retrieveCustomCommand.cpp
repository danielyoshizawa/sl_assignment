#include "retrieveCustomCommand.h"
#include "console.h"
#include "commandsName.h"

#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>

namespace command
{
	RetrieveCustomCommand::RetrieveCustomCommand(const std::shared_ptr<custom::Manager> & customManager) : customManager(customManager)
	{
	}

	message::message RetrieveCustomCommand::Execute(const std::vector<std::string> & params)
	{
		message::message msg;

		if (params.empty())
		{
			console::print(RETRIEVE_CUSTOM_CMD, "Missing custom object name", console::SEVERITY::WARNING);

			msg.method = "error";
			msg.params.push_back("401");
			msg.params.push_back("Object name was missing.");
		}
		else
		{
			std::shared_ptr<custom::custom> customObj = customManager->GetCustom(params.at(0));

			if (customObj == nullptr)
			{
				msg.method = "error";
				msg.params.push_back("401");
				msg.params.push_back("Object not found");
			}
			else
			{
				std::stringstream ss;
				{
					cereal::JSONOutputArchive oarchive(ss);
					oarchive(customObj);
				}

				msg.method = "success";
				msg.params.push_back("200");
				msg.params.push_back("Object found");
				msg.params.push_back(ss.str());

				console::print(RETRIEVE_CUSTOM_CMD, "Custom object sent", console::SEVERITY::NOTICE);
			}
		}

		return msg;
	}
}