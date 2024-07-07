#pragma once

#include <iostream>
#include <optional>
#include <string>
#include <vector>


namespace flection {

class clsDecl;

//TODO:

class decl
{
public:
    std::string name;
    inline decl(const std::string& _name)
        : name(_name)
    {}

protected:
    inline decl() {}
};

inline std::ostream& operator<<(std::ostream& out, const decl& A)
{
    out << "Decl name: " << A.name << "\n";
    return out;
}

class fieldDecl : public decl
{
public:
    std::string type;
};

inline std::ostream& operator<<(std::ostream& out, const fieldDecl& A)
{
    out << "field name: " << A.name << ", type: " << A.type << "\n";
    return out;
}

class funcDecl : public decl
{
public:
    std::vector<std::string> funcArgs;
    std::string              retType;
};

inline std::ostream& operator<<(std::ostream& out, const funcDecl& A)
{
    out << "function name: " << A.name << " -> " << A.retType << "(";
    for (auto&& i : A.funcArgs) {
        std::cout << i << " ";
    }
    std::cout << ")\n";
    return out;
}

class clsDecl : public decl
{
public:
    std::vector<fieldDecl>     fields;
    std::vector<funcDecl>      funcs;
    std::optional<std::string> base;
    std::string                fileIncluded;
    inline clsDecl() {}
    inline clsDecl(const char* s)
        : decl(s)
    {}
    inline clsDecl(const std::string& s)
        : decl(s)
    {}
};

inline std::ostream& operator<<(std::ostream& out, const clsDecl& A)
{
    out << "Class decl in: " << A.fileIncluded << "\n" << A.name;
    if (A.base != std::nullopt) {
        std::cout << " : " << A.base.value();
    }
    std::cout << "{\n";
    for (auto&& i : A.fields) {
        std::cout << "  " << i;
    }
    for (auto&& i : A.funcs) {
        std::cout << "  " << i;
    }
    std::cout << "}\n";
    return out;
}

const static inline clsDecl _IntDecl("int");
const static inline clsDecl _DoubleDecl("double");
const static inline clsDecl _FloatDecl("float");
const static inline clsDecl _CharDecl("char");
const static inline clsDecl _ShortDecl("short");
const static inline clsDecl _stdStringDecl("std::string");

}   // namespace flection
