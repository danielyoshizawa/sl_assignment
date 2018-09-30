#pragma once
#ifndef CONSOLE_H
#define CONSOLE_H

#include <iostream>
#include <string>

namespace console
{
	enum SEVERITY
	{
		NOTICE,
		WARNING,
		ERROR_S
	};

	inline void print(const std::string & id, const std::string & message, SEVERITY severity = NOTICE)
	{
		char severity_ = 'N';

		switch (severity)
		{
		case NOTICE:
			severity_ = 'N';
			break;
		case WARNING:
			severity_ = 'W';
			break;
		case ERROR_S:
			severity_ = 'E';
			break;
		default:
		{
			severity_ = 'N';
		}
		}

		std::cout << ":: "<< severity_ << " :: " << id << " :: " << message << " ::" << std::endl;
	}
}

#endif // CONSOLE_H
