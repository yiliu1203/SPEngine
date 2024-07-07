#pragma once
#include "cursorWrapper.hpp"
#include "metaDefs.hpp"
#include "reflectionDecl.hpp"
#include "clang-c/Index.h"
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>


namespace flection {

class metaClassParser
{
public:
    metaClassParser();
    ~metaClassParser() {};
    std::vector<clsDecl> parse(const flection::cursor& root);

private:
    std::optional<clsDecl> clsMetaCursorParse(const cursor&) const;
    funcDecl               funcCursorParse(const cursor&) const;
    fieldDecl              fieldCursorParse(const cursor&) const;
};
}   // namespace flection