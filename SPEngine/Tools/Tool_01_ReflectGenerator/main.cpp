#include "clang-c/CXString.h"
#include "clang-c/Index.h"
#include <iostream>
#include <string>


std::string getString(const CXString& cxs)
{
    std::string ret(clang_getCString(cxs));
    clang_disposeString(cxs);
    return ret;
}

int main(int argc, const char** argv)
{
    CXIndex index{clang_createIndex(1, 1)};
    auto    result = clang_parseTranslationUnit(
        index,
        "D:/myWorkVirtualZ/SPingEngine/Github_SPEngineFrom0/SPEngine/Tools/Tool_01_ReflectGenerator/Test_cpp.cpp",
        // "D:/myWorkVirtualZ/SPingEngine/Github_SPEngineFrom0/SPEngine/Sandbox/SandboxApp.cpp",
        nullptr,
        0,
        nullptr,
        0,
        0);


#ifdef __clang__
    std::cout << "Define Clang\n";
#endif

    auto cur = clang_getTranslationUnitCursor(result);
    clang_visitChildren(
        cur,
        [](CXCursor cur, CXCursor par, CXClientData data) {
            std::cout << " Cursor name:" << getString(clang_getCursorDisplayName(cur));
            std::cout << " Cursor type:" << getString(clang_getTypeSpelling(clang_getCursorType(cur))) << "\n";
            return CXChildVisit_Recurse;
        },
        nullptr);
    return 0;
}