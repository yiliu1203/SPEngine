#pragma once

#include "_Macro_.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>


namespace SP {

class SP_API Log
{

public:
    static Log& Instance()
    {
        static Log myLog;
        return myLog;
    }
    inline std::shared_ptr<spdlog::logger> GetCoreLogger() const { return m_CoreLogger; }
    inline std::shared_ptr<spdlog::logger> GetClientLogger() const { return m_ClientLogger; }

private:
    std::shared_ptr<spdlog::logger> m_CoreLogger;
    std::shared_ptr<spdlog::logger> m_ClientLogger;
    Log()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        m_CoreLogger = spdlog::stdout_color_mt("Core");
        m_CoreLogger->set_level(spdlog::level::trace);
        m_ClientLogger = spdlog::stdout_color_mt("Client");
        m_ClientLogger->set_level(spdlog::level::trace);
    }
};


// -----------------------Log-------------------------


#define CORE_INFO(...) Log::Instance().GetCoreLogger()->info(__VA_ARGS__)
#define CORE_WARN(...) Log::Instance().GetCoreLogger()->warn(__VA_ARGS__)
#define CORE_ERROR(...) Log::Instance().GetCoreLogger()->error(__VA_ARGS__)

#define SP_INFO(...) Log::Instance().GetCoreLogger()->info(__VA_ARGS__)
#define SP_WARN(...) Log::Instance().GetCoreLogger()->warn(__VA_ARGS__)
#define SP_TRACE(...) Log::Instance().GetCoreLogger()->warn(__VA_ARGS__)
#define SP_ERROR_(...) Log::Instance().GetCoreLogger()->error(__VA_ARGS__)   // SP_ERROR 和系统预定义的有冲突


///----------------------------------------------------------------------



//     CORE_ERROR("{0}  {1}", "ccccc", "fffffffff");

}   // namespace SP