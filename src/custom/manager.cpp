#include "manager.h"
#include <iostream>

namespace custom
{
	void Manager::CreateCustom(const std::string & name, const std::string & message)
	{
		custom_sptr customObj = std::make_shared<custom>(name, message);
		customsMap.insert(std::pair<std::string, custom_sptr>(name, customObj));
	}

	void Manager::AddCustom(custom_sptr customObj)
	{
		customsMap.insert(std::pair<std::string, custom_sptr>(customObj->getName(), customObj));
	}

	custom_sptr Manager::GetCustom(const std::string & name) const
	{
		customs_map_t::const_iterator found = customsMap.find(name);

		if (found != customsMap.end())
		{
			return found->second;
		}

		return nullptr;
	}

	std::vector<std::string> Manager::GetListOfNames()	
	{
		std::vector<std::string> customsNames;
		for (customs_map_t::iterator it = customsMap.begin(); it != customsMap.end(); ++it)
		{
			customsNames.push_back(it->first);
		}

		return customsNames;
	}
}