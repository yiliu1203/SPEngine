#include "rflParser.hpp"
#include "utils.hpp"
#include "clang-c/CXString.h"
#include "clang-c/Index.h"
#include <iostream>
#include <string>



// int main(int argc, const char** argv)
// {

//     CXIndex index{clang_createIndex(1, 1)};
//     auto    result = clang_parseTranslationUnit(
//         index,
//         "D:/myWorkVirtualZ/SPingEngine/Github_SPEngineFrom0/SPEngine/Tools/Tool_01_ReflectGenerator/Test_cpp.cpp",
//         // "D:/myWorkVirtualZ/SPingEngine/Github_SPEngineFrom0/SPEngine/Sandbox/SandboxApp.cpp",
//         nullptr,
//         0,
//         nullptr,
//         0,
//         0);



// #ifdef __clang__
//     std::cout << "Define Clang\n";
// #endif

//     auto cur = clang_getTranslationUnitCursor(result);
//     clang_visitChildren(
//         cur,
//         [](CXCursor cur, CXCursor par, CXClientData data) {
//             std::cout << " Cursor name:" << flection::clangUtils::getString(clang_getCursorDisplayName(cur));
//             std::cout << " Cursor type:" << flection::clangUtils::getString(clang_getTypeSpelling(clang_getCursorType(cur)))
//                       << "\n";
//             return CXChildVisit_Recurse;
//         },
//         nullptr);
//     return 0;
// }

int main(int argc, const char** argv)
{
    flection::rflParser parser;
    parser.parseFile("D:/myWorkVirtualZ/SPingEngine/Github_SPEngineFrom0/SPEngine/Tools/Tool_01_ReflectGenerator/Test_cpp.cpp");
    auto result = parser.getResult();
    return 0;
}