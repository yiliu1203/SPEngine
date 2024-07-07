namespace flection {
template <class T>
class FlectionGenCls
{};
}   // namespace flection
#define RFL_ALL(CLASS_NAME) friend class flection::FlectionGenCls<CLASS_NAME>;
#ifdef __clang__
#    define RFL_CLASS(...) __attribute__((annotate("reflect_class", #__VA_ARGS__)))
#    define RFL_PROPERTY(...) __attribute__((annotate("reflect_property", #__VA_ARGS__)))
#    define RFL_FUNC(...) __attribute__((annotate("reflect_func", #__VA_ARGS__)))
#else
#    define RFL_CLASS(...)
#    define RFL_PROPERTY(...)
#    define RFL_FUNC(...)
#endif

#include <string>

class RFL_CLASS() students
{
public:
    students(long long _uid, std::string name)
    {
        uid         = _uid;
        this->name  = name;
        PrivateInfo = "Private";
    }
    RFL_ALL(students);

public:
    RFL_PROPERTY()
    long long uid;
    RFL_PROPERTY()
    std::string name;

private:
    RFL_PROPERTY()
    std::string PrivateInfo;
    RFL_PROPERTY()
    students* stu;
};
