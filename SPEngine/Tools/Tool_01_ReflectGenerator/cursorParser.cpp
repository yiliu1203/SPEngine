#include "cursorParser.hpp"
#include "cursorWrapper.hpp"
#include "metaDefs.hpp"
#include "reflectionDecl.hpp"
#include "clang-c/Index.h"
#include <optional>
#include <string>
#include <vector>

const static std::string RFL_FUNC_  = "reflect_func";
const static std::string RFL_FIELD_ = "reflect_property";
const static std::string RFL_CLS_   = "reflect_class";

namespace flection {
metaClassParser::metaClassParser() {}

fieldDecl metaClassParser::fieldCursorParse(const cursor& csr) const
{
    fieldDecl fdl;
    fdl.name = csr.getDisplayName();
    fdl.type = csr.getType().getTypeName();
    return fdl;
}

funcDecl metaClassParser::funcCursorParse(const cursor& csr) const
{
    funcDecl fdl;
    fdl.name    = csr.getSpelling();
    auto type   = csr.getType();
    fdl.retType = type.getRetType().getTypeName();
    int t       = type.getArgNums();
    for (int i = 0; i < t; t--) {
        fdl.funcArgs.push_back(type.getArgType(i).getTypeName());
    }
    return fdl;
}

std::optional<clsDecl> metaClassParser::clsMetaCursorParse(const cursor& csr) const
{
    // TODO: 当前cursorKind不是类声明或者结构体声明的错误处理


    auto cursor2ClsChldren = csr.getChildrenWithDeclKind();
    // 不是反射类
    if (cursor2ClsChldren.size() <= 2) {
        return std::nullopt;
    }
    if (cursor2ClsChldren[0].getDisplayName() != RFL_CLS_) {
        return std::nullopt;
    }
    clsDecl ret      = clsDecl(csr.getType().getTypeName());
    ret.fileIncluded = csr.getCodeLocInfo().sourceFile;
    if (cursor2ClsChldren[1].getKind() == CXCursor_CXXBaseSpecifier) {
        ret.base = cursor2ClsChldren[1].getDisplayName();
    }
    for (int i = 1; i < cursor2ClsChldren.size(); i++) {
        auto curName = cursor2ClsChldren[i].getDisplayName();
        if (curName == RFL_FIELD_) {
            ret.fields.push_back(this->fieldCursorParse(cursor2ClsChldren[i - 1]));
        }
        else if (curName == RFL_FUNC_) {
            ret.funcs.push_back(this->funcCursorParse(cursor2ClsChldren[i - 1]));
        }
    }
    return ret;
}

std::vector<clsDecl> metaClassParser::parse(const flection::cursor& root)
{
    auto                 declList = root.getAllChildrenStructDeclCursor();
    std::vector<clsDecl> ret;
    for (auto&& i : declList) {
        auto cls = this->clsMetaCursorParse(i);
        if (cls != std::nullopt) {
            ret.push_back(cls.value());
        }
    }
    return ret;
}

}   // namespace flection
