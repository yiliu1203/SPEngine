#pragma once
#include "cursorWrapper.hpp"
#include "index.hpp"
#include <string>
#include <unordered_map>
#include <vector>

namespace flection {

class fileUnit
{
public:
    fileUnit(const std::string& fileName, std::vector<std::string>& parserArgs, flection::index _index,
             CXTranslationUnit_Flags options = CXTranslationUnit_None);
    const cursor       getRootCursor() const;
    const std::string& getFileName() const;
    ~fileUnit();

private:
    CXTranslationUnit unit;
    index             m_index;
    std::string       _fileName;
};

}   // namespace flection
