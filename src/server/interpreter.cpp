#include "interpreter.h"

#include <string>
#include <vector>

namespace
{
	blocks_vector_type splitBlocks(const std::wstring & buffer)
	{
		blocks_vector_type blocksVector;
		std::string::size_type cursor = 0;
		std::wstring temp = buffer;

		while (cursor != std::string::npos)
		{
			std::string::size_type openBracket = temp.find_first_of(L"[", cursor);
			std::string::size_type closeBracket = temp.find_first_of(L"]", cursor);

			if (openBracket != std::string::npos || closeBracket != std::string::npos)
			{
				std::wstring block = temp.substr(openBracket + 1, (closeBracket - 1) - openBracket);
				blocksVector.push_back(block);

				cursor = closeBracket + 1;
			}
			else
			{
				cursor = std::string::npos;
			}
		}

		return blocksVector;
	}

	pairs_vector_type splitPairs(const blocks_vector_type & blocksVector)
	{
		pairs_vector_type pairsVector;

		for (std::wstring block : blocksVector)
		{
			std::string::size_type pipePos = block.find_first_of(L"|");
			std::wstring first = block.substr(0, pipePos);
			std::wstring second = block.substr(pipePos + 1);
			pairsVector.push_back(pair_type(first, second));
		}

		return pairsVector;
	}
}

namespace server
{
	interpreter::~interpreter()
	{

	}

	interpreter::interpreter()
	{

	}

	pairs_vector_type interpreter::read(const std::wstring buffer)
	{
		blocks_vector_type blocksVector = splitBlocks(buffer);
		pairs_vector_type pairsVector = splitPairs(blocksVector);

		return pairsVector;
	}
}