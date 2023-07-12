#pragma once

#include <filesystem>
#include "Log.h"

#define EXPAND_MACRO(x) x
#define STRINGIFY_MACRO(x) #x


#define KYSI_ASSERT_IMPL(check, msg, ...) { if(!(check)) { Log::error(msg, __VA_ARGS__); __debugbreak(); } }
#define KYSI_ASSERT_WITH_MSG(check, ...) KYSI_ASSERT_IMPL(check, "Assertion failed: {0}", __VA_ARGS__)
#define KYSI_ASSERT_NO_MSG(check) KYSI_ASSERT_IMPL(check, "Assertion '{0}' failed at {1}:{2}", STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define KYSI_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define KYSI_ASSERT_GET_MACRO(...) EXPAND_MACRO( KYSI_ASSERT_GET_MACRO_NAME(__VA_ARGS__, KYSI_ASSERT_WITH_MSG, KYSI_ASSERT_NO_MSG) )

#define KYSE_ASSERT(...) EXPAND_MACRO(KYSI_ASSERT_GET_MACRO(__VA_ARGS__)(__VA_ARGS__))
