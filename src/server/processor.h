#pragma once
#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "message.h"
#include "command.h"
#include <map>
#include "manager.h"

namespace server
{
	class Processor
	{
		typedef std::shared_ptr<command::Command> command_sptr;
		typedef std::map<std::string, command_sptr> command_map;
	private:
		command_map commandsMap;
		std::shared_ptr<custom::Manager> customManager;
	public:
		virtual ~Processor() {};
		Processor();
		std::string Process(const std::string & buffer);
	private:
		void initializeCommands();
		message::message executeCommand(const message::message & msg);
	};
}

#endif // PROCESSOR_H