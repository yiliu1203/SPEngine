#pragma once
#ifndef METADEFS_HPP_
#    define METADEFS_HPP_
namespace flection {
template <class T>
class FlectionGenCls
{};
}   // namespace flection
#    define RFL_ALL(CLASS_NAME) friend class flection::FlectionGenCls<CLASS_NAME>;
#    ifdef __clang__
#        define RFL_CLASS(...) __attribute__((annotate("reflect_class", #__VA_ARGS__)))
#        define RFL_PROPERTY(...) __attribute__((annotate("reflect_property", #__VA_ARGS__)))
#        define RFL_FUNC(...) __attribute__((annotate("reflect_func", #__VA_ARGS__)))
#    else
#        define RFL_CLASS(...)
#        define RFL_PROPERTY(...)
#        define RFL_FUNC(...)
#    endif
#endif