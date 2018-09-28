
#pragma once
#ifndef CUSTOM_H
#define CUSTOM_H

#include <string>
#include <cereal/access.hpp>

namespace custom
{
	class custom
	{
	private:
		std::string name;
		std::string message;
	public:
		virtual ~custom();
		custom(std::string name, std::string message);
		custom();
		void printInfo();
		void setName(const std::string & _name);
		void setMessage(const std::string & _message);
		std::string getName() const;
		std::string getMessage() const;

		friend class cereal::access;

		template<class Archive>
		void serialize(Archive & ar)
		{
			ar(name, message);
		};
	};
};

#endif // CUSTOM_H