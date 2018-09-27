#pragma once
#ifndef INTERPRETER_H
#define INTERPRETER_H

/*
	[method|<function>]
	[param01|<data>]
	[param02|<data>]
	[param03|<data>]
	...
	[paramYX|<data>]

	eg.

	[method|createCustom][param01|custom01][param02|First Custom]
	
	createCustom is the method that the client is trying to call
	custom01 is the name of the custom object
	First Custom is the message for the new custom object
*/

#include <vector>

typedef std::vector<std::wstring> blocks_vector_type;
typedef std::pair<std::wstring, std::wstring> pair_type;
typedef std::vector<pair_type> pairs_vector_type;

namespace server
{
	class interpreter
	{
	public:
		virtual ~interpreter();
		interpreter();
		pairs_vector_type read(const std::wstring buffer);
	};
};

#endif // INTERPRETER_H