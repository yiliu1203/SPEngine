
#pragma once

#ifndef __MACRO_H
#    define __MACRO_H


// ---------------BIT--------------------

#    define BIT(x) 1 << x

//----------------------------------------


// -------ASSERT--------------------------

#    ifndef CUSTOM_ERROR
#        define CUSTOM_ERROR(...)                                                                         \
            {                                                                                             \
                CustomException exception = CustomException(__FILE__, __LINE__, __func__, {__VA_ARGS__}); \
                SP_ERROR_(exception.what());                                                              \
                throw exception;                                                                          \
            }

#        define ASSERT(condition, ...)        \
            {                                 \
                if (!(condition)) {           \
                    CUSTOM_ERROR(__VA_ARGS__) \
                }                             \
            }

#        define SP_ASSERT(condition, ...)     \
            {                                 \
                if (!(condition)) {           \
                    CUSTOM_ERROR(__VA_ARGS__) \
                }                             \
            }

#        define DX_ASSERT(hrcall, ...)         \
            DxgiInfoManager::Instance().Set(); \
            if (FAILED(hr = (hrcall))) {       \
                CUSTOM_ERROR(__VA_ARGS__)      \
            }

#    endif
// ------------------------------------------------


#    define NoCopyable(clz)                  \
        clz(clz&)                  = delete; \
        clz& operator=(clz&)       = delete; \
        clz(const clz&)            = delete; \
        clz& operator=(const clz&) = delete;

#endif
