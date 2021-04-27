#pragma once
#include <string>
#include <string_view>
#include <fstream>
#include "../Core/Assert.hh"

namespace rym
{
	namespace utils
	{
		static std::string read_file(const std::string_view& filepath)
		{
			std::string result;
			//std::ifstream in(filepath, std::ios::binary); // ifstream closes itself due to RAII
			std::ifstream in(filepath.data(), std::ios::in | std::ios::binary); // ifstream closes itself due to RAII
			if (in)
			{
				in.seekg(0, std::ios::end);
				size_t size = in.tellg();
				if (size != -1)
				{
					result.resize(size);
					in.seekg(0, std::ios::beg);
					in.read(&result[0], size);
				}
				else
				{
					//RYM_CORE_ASSERT("Could not read from file '{0}'", filepath);
					RYM_CORE_ERROR("Could not read from file '{0}'", filepath);
				}
			}
			else
			{
				RYM_CORE_ERROR("Could not read from file '{0}'", filepath);
			}

			return result;
		}
	}
}