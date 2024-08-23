#include "cursorWrapper.hpp"
#include "utils.hpp"
#include "clang-c/CXFile.h"
#include "clang-c/Index.h"
#include <iostream>
#include <vector>

namespace flection {
cursor::cursor(const CXCursor& cursor) noexcept
    : _cursor(cursor)
{}

std::string cursor::getDisplayName() const noexcept
{
    return clangUtils::getString(clang_getCursorDisplayName(this->_cursor));
}

std::string cursor::getSpelling() const noexcept
{
    return clangUtils::getString(clang_getCursorSpelling(this->_cursor));
}

CXCursorKind cursor::getKind() const noexcept
{
    return (this->_cursor.kind);
}

std::vector<cursor> cursor::getChildrenWithDeclKind() const noexcept
{
    std::vector<cursor> ret;
    auto                visitorFunc = [](CXCursor current_cursor, CXCursor parent, CXClientData client_data) {
        auto childrenPtr = static_cast<std::vector<cursor>*>(client_data);
        auto curCursor   = flection::cursor(current_cursor);
        auto curKind     = curCursor.getKind();
        if (curKind != CXCursor_AnnotateAttr && curKind != CXCursor_FieldDecl && curKind != CXCursor_CXXMethod &&
            curKind != CXCursor_FunctionDecl && curKind != CXCursor_CXXBaseSpecifier) {
            return CXChildVisit_Continue;
        }
        childrenPtr->emplace_back(curCursor);
        return CXChildVisit_Recurse;
    };
    clang_visitChildren(this->_cursor, visitorFunc, &ret);
    return ret;
}

CursorList cursor::getAllChildrenStructDeclCursor() const noexcept
{
    std::vector<cursor> ret;
    auto                visitorFunc = [](CXCursor current_cursor, CXCursor parent, CXClientData client_data) {
        auto childrenPtr = static_cast<std::vector<cursor>*>(client_data);
        auto curCursor   = flection::cursor(current_cursor);
        auto curKind     = curCursor.getKind();
        if (curKind != CXCursor_ClassDecl && curKind != CXCursor_StructDecl) {
            return CXChildVisit_Recurse;
        }
        else {
            childrenPtr->emplace_back(current_cursor);
            return CXChildVisit_Continue;
        }
    };
    clang_visitChildren(this->_cursor, visitorFunc, &ret);
    return ret;
}

bool cursor::isDefinition() const noexcept
{
    return clang_isCursorDefinition(this->_cursor);
}

cursorType cursor::getType() const noexcept
{
    return cursorType(clang_getCursorType(this->_cursor));
}

CXCursor cursor::getCursor() const noexcept
{
    return this->_cursor;
}

codeInfo cursor::getCodeLocInfo() const noexcept
{
    codeInfo info;
    auto     range = clang_Cursor_getSpellingNameRange(this->_cursor, 0, 0);
    auto     start = clang_getRangeStart(range);
    CXFile   file;
    clang_getFileLocation(start, &file, &info.row, &info.col, &info.offset);
    info.sourceFile = clangUtils::getString(clang_getFileName(file));
    return info;
}

cursorType::cursorType(const CXType& type) noexcept
    : _type(type)
{}

std::string cursorType::getTypeName() const noexcept
{
    return clangUtils::getString(clang_getTypeSpelling(this->_type));
}

cursor cursorType::getDeclCursor() const noexcept
{
    return clang_getTypeDeclaration(this->_type);
}

int cursorType::getArgNums() const noexcept
{
    return clang_getNumArgTypes(this->_type);
}

cursorType cursorType::getArgType(unsigned index) const noexcept
{
    return clang_getArgType(this->_type, index);
}

bool cursorType::isConst() const noexcept
{
    return clang_isConstQualifiedType(this->_type);
}

CXTypeKind cursorType::getKind() const noexcept
{
    return this->_type.kind;
}

cursorType cursorType::getRetType() const noexcept
{
    return clang_getResultType(this->_type);
}

}   // namespace flection
