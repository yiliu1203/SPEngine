#pragma once

#include "PCH.h"
#include "FixedString.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"



namespace SP {

template <FixedString s>
class SP_API Log2
{

public:
    static constexpr FixedString    s_Name = s;
    std::shared_ptr<spdlog::logger> m_Logger;
    Log2()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        std::string tmp = Log2::s_Name.m_Data;
        m_Logger        = spdlog::stdout_color_mt(tmp);
        m_Logger->set_level(spdlog::level::trace);
    }
};



using CoreLog = Log2<"Core2">;

// extern CoreLog log;








}   // namespace SP