#pragma once
#ifndef PRINTCOMMAND_H
#define PRINTCOMMAND_H

#include "command.h"

namespace command
{
	class PrintCommand : public Command
	{
	public:
		virtual ~PrintCommand() {};
		PrintCommand() {};
		message::message Execute(const std::vector<std::string> & params) override;
	};
};

#endif // PRINTCOMMAND_H