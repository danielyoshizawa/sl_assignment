#pragma once
#ifndef COMMAND_H
#define COMMAND_H

#include "message.h"

namespace command
{
	class Command
	{
	public:
		virtual message::message Execute(const std::vector<std::string> & params) = 0;
	};
};

#endif // COMMAND_H