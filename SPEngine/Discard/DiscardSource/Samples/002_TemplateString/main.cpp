#include <iostream>


// 测试字符串常量作为模板参数

template <size_t N>
struct FixedString
{
    char data_[N];
    constexpr FixedString(const char (&array)[N]) { std::copy(array, array + N, data_); }
};

// 下面这个推导指引其实式不必要的  msvc 下不需要
template <size_t N>
FixedString(const char (&)[N]) -> FixedString<N>;

// template <size_t>
// struct TestTemplateStruct
// {
//     // void info() { std::cout << S.data_ << std::endl; }
// };

template <FixedString S>
struct TestTemplateStruct
{
    void info() { std::cout << S.data_ << std::endl; }
};



template <typename S>
struct _TestTemplateStruct2
{
    // void info() { std::cout << S.data_ << std::endl; }
};

template <size_t N>
struct TestTemplateStruct2
{
    // void info() { std::cout << S.data_ << std::endl; }
};


struct ttt
{
    /* data */
};


int main()
{
    std::cout << "Hello" << std::endl;

    auto t1 = TestTemplateStruct<"aaa">{};

    auto t2 = TestTemplateStruct<"bbb">{};

    auto t3 = TestTemplateStruct<"aaa">{};

    std::cout << "typeid t1: " << typeid(t1).hash_code() << std::endl;
    std::cout << "typeid t1: " << typeid(t2).hash_code() << std::endl;
    std::cout << "typeid t1: " << typeid(t3).hash_code() << std::endl;

    auto t4 = TestTemplateStruct2<1>{};
    // auto t5 = TestTemplateStruct2<ttt>{};

    t1.info();
    return 0;
}