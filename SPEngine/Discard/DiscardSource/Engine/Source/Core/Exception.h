#pragma once

#include <iostream>
#include <sstream>
#include <string_view>
#include <vector>


namespace SP {

class CustomException : public std::exception
{
public:
    CustomException(const std::string& _file_, size_t _line_, const std::string& _func_,
                    std::initializer_list<std::string> initial_list)
    {
        std::stringstream ss;
        // (ss << a << " ")...;
        for (auto item : initial_list) {
            ss << item << " ";
        }
        ss << "file:" << _file_ << " ";
        ss << "line:" << _line_ << " ";
        ss << "func:" << _func_ << " ";
        ss << std::endl;
        msg_ = ss.str();
    }

    const char* what() { return msg_.c_str(); };

private:
    std::string msg_;
};

// -------ASSERT--------------------------

#ifndef CUSTOM_ERROR
#    define CUSTOM_ERROR(...)                                                                         \
        {                                                                                             \
            CustomException exception = CustomException(__FILE__, __LINE__, __func__, {__VA_ARGS__}); \
            std::cout << exception.what();                                                            \
            throw exception;                                                                          \
        }

#    define ASSERT(condition, ...)        \
        {                                 \
            if (!(condition)) {           \
                CUSTOM_ERROR(__VA_ARGS__) \
            }                             \
        }

#    define sp_assert(condition, ...)     \
        {                                 \
            if (!(condition)) {           \
                CUSTOM_ERROR(__VA_ARGS__) \
            }                             \
        }
#endif
// ------------------------------------------------

}   // namespace SP