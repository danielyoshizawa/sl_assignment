#pragma once
#ifndef MANAGER_H
#define MANAGER_H

#include "custom.h"
#include <map>
#include <string>

namespace custom
{
	typedef std::shared_ptr<custom> custom_sptr;
	typedef std::map<std::string, custom_sptr> customs_map_t;

	class Manager
	{
	private:
		customs_map_t customsMap;
	public:
		virtual ~Manager() {};
		Manager() {};
		void CreateCustom(const std::string & name, const std::string & message);
		void AddCustom(custom_sptr customObj);
		custom_sptr GetCustom(const std::string & name) const;
		std::vector<std::string> GetListOfNames();
	};
};

#endif // MANAGER_H