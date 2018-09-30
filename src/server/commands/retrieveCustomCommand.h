#pragma once
#ifndef RETRIEVECUSTOMCOMMAND_H
#define RETRIEVECUSTOMCOMMAND_H

#include "command.h"
#include "manager.h"

namespace command
{
	class RetrieveCustomCommand : public Command
	{
	private:
		std::shared_ptr<custom::Manager> customManager;
	public:
		virtual ~RetrieveCustomCommand() {};
		RetrieveCustomCommand(const std::shared_ptr<custom::Manager> & customManager);
		message::message Execute(const std::vector<std::string> & params) override;
	};
};

#endif // RETRIEVECUSTOMCOMMAND_H