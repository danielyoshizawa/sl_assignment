#pragma once
#ifndef MANAGER_H
#define MANAGER_H

#include <Windows.h>
#include <memory>
#include "server.h"
#include "message.h"
#include "custom.h"
#include <map>

typedef std::shared_ptr<custom::custom> custom_sprt_type;
typedef std::pair<std::string, custom_sprt_type> custom_pair_type;

namespace server
{
	class manager
	{
	private:
		std::shared_ptr<server> server_sptr;
		std::map<std::string, custom_sprt_type> customsMap;
		std::vector<message::message> msgVector;
		bool running;
	public:
		virtual ~manager();
		manager();
		void start();
	private:
		void process(const message::message & msg);
		void createCustom(const std::string & name, const std::string & message);
	};
};

#endif // MANAGER_H