#pragma once

#include "../Plataform.hh"

#ifdef RYM_DEBUG
	#if defined(RYM_PLATFORM_WINDOWS)
		#define RYM_DEBUGBREAK() __debugbreak()
	#elif defined(RYM_PLATFORM_LINUX)
		#include <signal.h>
		#define RYM_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif

	#define RYM_ENABLE_ASSERTS
#else
	#define RYM_DEBUGBREAK()
#endif

#define RYM_EXPAND_MACRO(x) x
#define RYM_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define RYM_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }


// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#pragma warning(pop)


#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"

namespace rym {

	class Log
	{
	public:
		static void init();

		static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternio)
{
	os << glm::to_string(quaternio);
	return os;
}

// Core log macros
#define RYM_CORE_TRACE(...)    ::rym::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define RYM_CORE_INFO(...)     ::rym::Log::GetCoreLogger()->info(__VA_ARGS__)
#define RYM_CORE_WARN(...)     ::rym::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define RYM_CORE_ERROR(...)    ::rym::Log::GetCoreLogger()->error(__VA_ARGS__)
#define RYM_CORE_CRITICAL(...) ::rym::Log::GetCoreLogger()->critical(__VA_ARGS__)


// Client log macros
#define RYM_TRACE(...)         ::rym::Log::GetClientLogger()->trace(__VA_ARGS__)
#define RYM_INFO(...)          ::rym::Log::GetClientLogger()->info(__VA_ARGS__)
#define RYM_WARN(...)          ::rym::Log::GetClientLogger()->warn(__VA_ARGS__)
#define RYM_ERROR(...)         ::rym::Log::GetClientLogger()->error(__VA_ARGS__)
#define RYM_CRITICAL(...)      ::rym::Log::GetClientLogger()->critical(__VA_ARGS__)