#pragma once
#ifndef CREATECUSTOMCOMMAND_H
#define CREATECUSTOMCOMMAND_H

#include "command.h"
#include "manager.h"
#include "custom.h"

namespace command 
{
	class CreateCustomCommand : public Command
	{
	private:
		std::shared_ptr<custom::Manager> customManager;
	public:
		virtual ~CreateCustomCommand() {};
		CreateCustomCommand(const std::shared_ptr<custom::Manager> & customManager);
		message::message Execute(const std::vector<std::string> & params) override;
	};
};

#endif