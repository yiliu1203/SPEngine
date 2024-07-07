#pragma once
#include "clang-c/CXString.h"
#include "clang-c/Index.h"
#include <string>
#include <sys/stat.h>
#include <vector>



namespace flection {

class utils
{
public:
    static std::vector<std::string> splitStringBySpace(const std::string& str)
    {
        std::vector<std::string> ret;
        int                      last  = 0;
        int                      count = 0;
        for (int i = 0; i < str.size(); ++i, count++) {
            if (str[i] == ' ') {
                ret.push_back(str.substr(last, count));
                count = -1;
                last  = i + 1;
            }
        }
        ret.push_back(str.substr(last, count));
        return ret;
        ;
    }
    static bool hasNamespaceInClsName(std::string& str)
    {
        bool flag = false;
        for (auto i = 0; i < str.size(); ++i) {
            if (str[i] == ':') {
                flag   = true;
                str[i] = '_';
            }
        }
        return flag;
    }
};

class clangUtils
{
public:
    static std::string getString(const CXString& cxs)
    {
        std::string ret(clang_getCString(cxs));
        clang_disposeString(cxs);
        return ret;
    }
};
}   // namespace flection