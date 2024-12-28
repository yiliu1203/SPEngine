#include "ConfigVar.h"
#include <iostream>
#include <vector>


template <typename R, typename... Args>
class Delegate
{

public:
    using FProxy = R (*)(void*, Args...);

    class Handler
    {
    public:
        Handler(void* p, FProxy f)
            : receiver_(p)
            , fun_(f)
        {}

        void Execute(Args... args) { fun_(receiver_, args...); }

        bool operator==(const Handler& rhs) { return (receiver_ == rhs.receiver_ && fun_ == rhs.fun_); }

        void*  receiver_;
        FProxy fun_;
    };

    Delegate()
        : handlers_{}
    {}

    Delegate(const Handler& rhs)
        : handlers_(1, rhs)
    {}

    Delegate& operator+=(const Handler& rhs)
    {
        handlers_.push_back(rhs);
        return *this;
    }

    Delegate& operator-=(const Handler& rhs)
    {
        auto findIdx = std::find(handlers_.begin(), handlers_.end(), rhs);
        if (findIdx != handlers_.end()) {
            handlers_.erase(findIdx);
        }
        return *this;
    }

    Delegate& operator+=(const Handler&& rhs)
    {
        handlers_.push_back(rhs);
        return *this;
    }

    void Execute(Args... args)
    {
        for (auto item : handlers_) {
            item.Execute(args...);
        }
    }

    // F 作为普通函数指针，这里模板type 也可以是一个函数指针，既是类型，也是地址，类似字面量
    template <R (*F)(Args...)>
    static Handler FromFun()
    {
        return Handler(nullptr, &FromFunProxy<F>);
    }

    // CF 作为一个类的方法的指针
    template <typename C, R (C::*CF)(Args...)>
    static Handler FromMethod(C* c)
    {
        return Handler((void*)c, &FromMethodProxy<C, CF>);
    }

private:
    std::vector<Handler> handlers_;

    // 这里提供统一的 Handler 的调用
    template <R (*F)(Args...)>
    static R FromFunProxy(void*, Args... args)
    {
        return (*F)(args...);
    }

    // 这里提供统一的 Handler 的调用
    template <typename C, R (C::*CF)(Args...)>
    static R FromMethodProxy(void* c, Args... args)
    {
        return (((C*)c)->*CF)(args...);
    }
};



int add(int a, int b)
{
    std::cout << "add a + b =" << a + b << std::endl;
    return a + b;
}

class TestDelegateAdd
{
public:
    int add(int a, int b)
    {
        std::cout << "class add a + b =" << a + b << std::endl;
        return a + b;
    }
};

int main()
{

    Delegate<int, int, int> delegate;
    TestDelegateAdd         delegateAdd;
    delegate += Delegate<int, int, int>::FromFun<add>();
    delegate += Delegate<int, int, int>::FromMethod<TestDelegateAdd, &TestDelegateAdd::add>(&delegateAdd);

    delegate.Execute(2, 3);


    std::cout << "Hello World" << std::endl;
    return 0;
}