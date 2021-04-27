#pragma once

#include <chrono>
#include <functional>
#include <thread>
#include <any>

namespace rym
{
	typedef void(*TIMEOUT)();

	class Timer
	{
	public:
		Timer(TIMEOUT callback, double duration)
		{
			m_Callback = callback;
			m_Duration = duration;
		}

		~Timer()
		{
		}

		void start()
		{
			running();
		}
	private:
		void running()
		{
		}

		void stop()
		{
			m_Callback();
		}

	private:
		double m_Duration;
		TIMEOUT m_Callback;
		//std::thread t;
	};
}