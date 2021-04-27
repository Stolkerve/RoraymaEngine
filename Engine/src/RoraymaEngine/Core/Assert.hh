#pragma once

#include "Log.hh"
#include <filesystem>

#ifdef RYM_ENABLE_ASSERTS
	#define RYM_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { RYM##type##ERROR(msg, __VA_ARGS__); RYM_DEBUGBREAK(); } }
	#define RYM_INTERNAL_ASSERT_WITH_MSG(type, check, ...) RYM_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define RYM_INTERNAL_ASSERT_NO_MSG(type, check) RYM_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", RYM_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

	#define RYM_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define RYM_INTERNAL_ASSERT_GET_MACRO(...) RYM_EXPAND_MACRO( RYM_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, RYM_INTERNAL_ASSERT_WITH_MSG, RYM_INTERNAL_ASSERT_NO_MSG) )

	#define RYM_ASSERT(...) RYM_EXPAND_MACRO( RYM_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
	#define RYM_CORE_ASSERT(...) RYM_EXPAND_MACRO( RYM_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
	#define RYM_ASSERT(...)
	#define RYM_CORE_ASSERT(...)
#endif