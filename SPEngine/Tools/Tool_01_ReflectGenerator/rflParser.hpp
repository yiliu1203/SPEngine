#pragma once

#include "reflectionDecl.hpp"
#include <string>
#include <unordered_map>
#include <vector>
namespace flection {

class rflParser
{
public:
    rflParser() {}
    void                 parseFile(const std::string& filename) noexcept;
    std::vector<clsDecl> getResult() const;

private:
    std::vector<std::string>                 compileFlags;
    std::unordered_map<std::string, clsDecl> clsMap;
};
}   // namespace flection
