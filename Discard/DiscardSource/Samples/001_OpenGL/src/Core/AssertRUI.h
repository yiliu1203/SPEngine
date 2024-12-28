#pragma once

#include "APre.h"
#include "Exception.h"
#include <sys/stat.h>

NS_BEGIN

class AssertRUI
{
private:
    const std::string m_FilePath;   // 这里不能用 引用， 否则常量引用 初始化 到了一个将死值， 那有什么办法优化呢



public:
    AssertRUI(const std::string& filename)
        : m_FilePath{std::string{ROOT} + filename}
    {
        ASSERT(isFileExists_stat(m_FilePath), " File Not Exist :", m_FilePath.c_str());
    }
    AssertRUI(const AssertRUI& rui)
        : m_FilePath{rui.m_FilePath}
    {}

    AssertRUI(AssertRUI&& rui)
        : m_FilePath{std::move(rui.m_FilePath)}
    {}

    static bool isFileExists_stat(const std::string& filename)
    {
        struct stat buffer;
        return (stat(filename.c_str(), &buffer) == 0);
    }

public:
    const std::string& path() const { return m_FilePath; }
};

NS_END