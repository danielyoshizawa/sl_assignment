
#pragma once
#ifndef MESSAGE_H
#define MESSAGE_H

#include <vector>
#include <string>

namespace message
{
	struct message
	{
		std::string method;
		std::vector<std::string> params;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(method, params);
		};
	};
};
#endif // MESSAGE_H