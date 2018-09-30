#pragma once
#ifndef CUSTOMSLISTCOMMAND_H
#define CUSTOMSLISTCOMMAND_H

#include "command.h"
#include "manager.h"
#include "custom.h"

namespace command
{
	class CustomsListCommand : public Command
	{
	private:
		std::shared_ptr<custom::Manager> customManager;
	public:
		virtual ~CustomsListCommand() {};
		CustomsListCommand(const std::shared_ptr<custom::Manager> & customManager);
		message::message Execute(const std::vector<std::string> & params) override;
	};
};

#endif // CUSTOMSLISTCOMMAND_H