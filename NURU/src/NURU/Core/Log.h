#pragma once
#include <memory>

#include <NURU/Core/Core.h>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace NURU 
{
    class Log 
    {
    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    public:
        static void Init();
        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

    };
    
}

//Core Logging Macros
#define NURU_CORE_TRACE(...) ::NURU::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define NURU_CORE_INFO(...) ::NURU::Log::GetCoreLogger()->info(__VA_ARGS__)
#define NURU_CORE_WARN(...) ::NURU::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define NURU_CORE_ERROR(...) ::NURU::Log::GetCoreLogger()->error(__VA_ARGS__)
#define NURU_CORE_FATAL(...) ::NURU::Log::GetCoreLogger()->fatal(__VA_ARGS__)

//Client Logging Macros
#define NURU_TRACE(...) ::NURU::Log::GetClientLogger()->trace(__VA_ARGS__)
#define NURU_INFO(...) ::NURU::Log::GetClientLogger()->info(__VA_ARGS__)
#define NURU_WARN(...) ::NURU::Log::GetClientLogger()->warn(__VA_ARGS__)
#define NURU_ERROR(...) ::NURU::Log::GetClientLogger()->error(__VA_ARGS__)
#define NURU_FATAL(...) ::NURU::Log::GetClientLogger()->fatal(__VA_ARGS__)

#ifdef NURU_DIST

    #define NURU_CORE_TRACE
    #define NURU_CORE_INFO
    #define NURU_CORE_WARN
    #define NURU_CORE_ERROR
    #define NURU_CORE_FATAL
    #define NURU_TRACE
    #define NURU_INFO
    #define NURU_WARN
    #define NURU_ERROR
    #define NURU_FATAL

#endif