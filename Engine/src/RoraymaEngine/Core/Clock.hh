#pragma once

#include <chrono>
#include "Log.hh"

namespace rym
{
	class Profile
	{
	public:
		Profile(const char* scopeName)
		{
			m_Name = scopeName;
			m_Start = std::chrono::high_resolution_clock::now();
		}
		~Profile()
		{
			auto end = std::chrono::high_resolution_clock::now();
			auto msEnd = std::chrono::time_point_cast<std::chrono::microseconds>(end).time_since_epoch().count();
			auto msStart = std::chrono::time_point_cast<std::chrono::microseconds>(m_Start).time_since_epoch().count();
			RYM_CORE_INFO("The scope of {0} take: {1} ms", m_Name, (msEnd - msStart) * 0.001);
		}
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
		const char* m_Name;
	};

	#define RYM_PROFILE_SCOPE(NAME) rym::Profile(NAME)

	class Clock
	{
	public:
		Clock();

		//float getTimeElapsed() { return (end - m_start).count(); }

		//void restart() { end = std::chrono::high_resolution_clock::now(); }
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_start, end;
	};
}