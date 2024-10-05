#pragma once

#ifdef _WIN32
#    ifdef SP_DYNAMIC_DLL
#        ifdef SP_BUILD_DLL
#            define SP_API __declspec(dllexport)
#        else
#            define SP_API __declspec(dllimport)
#        endif
#    else
#        define SP_API
#    endif
#endif
