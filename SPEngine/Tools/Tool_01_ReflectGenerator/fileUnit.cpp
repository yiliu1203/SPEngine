
#include "fileUnit.hpp"
#include "cursorWrapper.hpp"
#include "clang-c/Index.h"
#include <string>
#include <vector>

namespace flection {

fileUnit::fileUnit(const std::string& fileName, std::vector<std::string>& parserArgs, flection::index _index,
                   CXTranslationUnit_Flags options)
    : _fileName(fileName)
    , m_index(_index)
{

    std::vector<const char*> args;
    for (auto&& t : parserArgs) {
        args.emplace_back(t.c_str());
    }
    this->unit = clang_parseTranslationUnit(
        m_index.getIndex(), _fileName.c_str(), args.size() > 0 ? &args[0] : nullptr, parserArgs.size(), 0, 0, options);
}

const cursor fileUnit::getRootCursor() const
{
    return clang_getTranslationUnitCursor(this->unit);
}

fileUnit::~fileUnit()
{
    clang_disposeTranslationUnit(this->unit);
    clang_disposeIndex(this->m_index.getIndex());
}

const std::string& fileUnit::getFileName() const
{
    return _fileName;
}
}   // namespace flection
