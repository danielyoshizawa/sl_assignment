#include "processor.h"

#include "console.h"
#include "commandsName.h"

#include "printCommand.h"
#include "createCustomCommand.h"
#include "customsListCommand.h"
#include "retrieveCustomCommand.h"

#include <sstream>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>

namespace server
{
	Processor::Processor()
	{
		customManager = std::make_shared<custom::Manager>();
		initializeCommands();
	}

	std::string Processor::Process(const std::string & buffer)
	{
		message::message msgIn;
		{
			std::stringstream ssIn;
			ssIn << buffer;

			cereal::JSONInputArchive iarchive(ssIn);
			iarchive(msgIn);
		}

		message::message msgOut = executeCommand(msgIn);

		std::stringstream ssOut;
		{
			cereal::JSONOutputArchive oarchive(ssOut);
			oarchive(msgOut);
		}

		return ssOut.str();
	}

	void Processor::initializeCommands()
	{
		commandsMap.insert(std::pair<std::string, command_sptr>(PRINT_CMD, std::make_shared<command::PrintCommand>()));
		commandsMap.insert(std::pair<std::string, command_sptr>(CREATE_CUSTOM_CMD, std::make_shared<command::CreateCustomCommand>(customManager)));
		commandsMap.insert(std::pair<std::string, command_sptr>(CUSTOM_LIST_CMD, std::make_shared<command::CustomsListCommand>(customManager)));
		commandsMap.insert(std::pair<std::string, command_sptr>(RETRIEVE_CUSTOM_CMD, std::make_shared<command::RetrieveCustomCommand>(customManager)));
	}

	message::message Processor::executeCommand(const message::message & msg)
	{
		command_map::iterator found = commandsMap.find(msg.method);

		if (found == commandsMap.end())
		{
			std::string error("Command not found");
			console::print("PROCESSOR", error, console::SEVERITY::ERROR_S);

			message::message response;
			response.method = "error";
			response.params.push_back("401");
			response.params.push_back("Command not found");

			return response;
		}

		return found->second->Execute(msg.params);
	}
}