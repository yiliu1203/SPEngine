#pragma once

#include "clang-c/Index.h"
#include <functional>
#include <string>
#include <vector>


namespace flection {

typedef struct
{
    std::string  sourceFile;
    unsigned int row;
    unsigned int col;
    unsigned int offset;
} codeInfo;

class cursorType;
class cursor;
typedef std::vector<cursor> CursorList;

class cursor
{
public:
    inline cursor() {}
    cursor(const CXCursor& cursor) noexcept;
    std::string  getDisplayName() const noexcept;
    std::string  getSpelling() const noexcept;
    CXCursorKind getKind() const noexcept;

    // get Children cursor like
    //  CXCursor_AnnotateAttr
    //  ||CXCursor_FieldDecl
    //  ||CXCursor_CXXMethod
    //  ...
    CursorList getChildrenWithDeclKind() const noexcept;
    CursorList getAllChildrenStructDeclCursor() const noexcept;
    bool       isDefinition() const noexcept;
    cursorType getType() const noexcept;
    codeInfo   getCodeLocInfo() const noexcept;
    CXCursor   getCursor() const noexcept;

private:
    CXCursor _cursor;
};

class cursorType
{
public:
    cursorType(const CXType& type) noexcept;
    std::string getTypeName() const noexcept;
    cursor      getDeclCursor() const noexcept;
    int         getArgNums() const noexcept;
    cursorType  getArgType(unsigned index) const noexcept;
    cursorType  getRetType() const noexcept;
    bool        isConst() const noexcept;
    CXTypeKind  getKind() const noexcept;

private:
    CXType _type;
};
}   // namespace flection
