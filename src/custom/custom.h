
#pragma once
#ifndef CUSTOM_H
#define CUSTOM_H

#include <string>

namespace custom
{
	class custom
	{
	private:
		std::string name;
		std::string message;
	public:
		virtual ~custom();
		custom();
		void printInfo();
		void setName(const std::string & _name);
		void setMessage(const std::string & _message);
		std::string getName() const;
		std::string getMessage() const;
	};
}

#endif // CUSTOM_H