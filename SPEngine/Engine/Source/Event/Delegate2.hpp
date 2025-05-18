#include <atomic>
#include <concepts>
#include <functional>
#include <vector>


namespace SP {

template <typename...>
class Delegate;

template <typename R, typename... Args>
class Delegate<R(Args...)>
{
    using DelegateType    = std::function<R(Args...)>;
    DelegateType callback = nullptr;

public:
    Delegate()                      = default;
    Delegate& operator=(Delegate&&) = default;
    Delegate(Delegate&&)            = default;
    ~Delegate()                     = default;

    Delegate(Delegate const&)            = default;
    Delegate& operator=(Delegate const&) = default;

    template <typename F>
    requires std::is_constructible_v<DelegateType, F>
    void BindLambda(F&& lambda)
    {
        // assert callback == nullptr
        callback = lambda;
    }

    void BindStatic(R (*pf)(Args...)) { callback = pf; }

    template <typename T>
    void BindMember(R (T::*mem_fn)(Args...), T& instance)
    {
        callback = [&instance, mem_fn](Args&&... args) { return (instance.*mem_fn)(std::forward<Args>(args)...); };
    }

    void Unbind() { callback = nullptr; }
    bool IsBound() const { return callback != nullptr; }

    R Execute(Args... args) const { return callback(args...); }

    R ExecuteIfBound(Args... args) const { return IsBound() ? callback(args...) : R(); }

    R operator()(Args... args) const { return callback(args...); }

    template <typename F>
    requires std::is_constructible_v<DelegateType, F>
    static Delegate CreateFromLambda(F&& lambda)
    {
        Delegate delegate;
        delegate.BindLambda(std::forward<F>(lambda));
        return delegate;
    }

    static Delegate CreateFromStatic(R (*pf)(Args...))
    {
        Delegate delegate;
        delegate.BindStatic(pf);
        return delegate;
    }

    template <typename T>
    static Delegate CreateFromMember(R (T::*mem_fn)(Args...), T& instance)
    {
        Delegate delegate;
        delegate.BindMember(mem_fn, instance);
        return delegate;
        // callback = [&instance, mem_fn](Args&&... args) { return (instance.*mem_fn)(std::forward<Args>(args)...); };
    }
};


class DelegateHandle
{
public:
    DelegateHandle()
        : id(INVALID_ID)
    {}
    explicit DelegateHandle(size_t id)
        : id(GenerateID())
    {}

    DelegateHandle& operator=(DelegateHandle&&) = default;
    DelegateHandle(DelegateHandle&&)            = default;

    DelegateHandle(DelegateHandle const&)            = default;
    DelegateHandle& operator=(DelegateHandle const&) = default;

    operator bool() const { return IsValid(); }

    bool operator==(DelegateHandle const& that) const { return id == that.id; }
    bool operator<(DelegateHandle const& that) const { return id < that.id; }

    bool IsValid() const { return id != INVALID_ID; }
    void Reset() { id = INVALID_ID; }

private:
    size_t                  id;
    static constexpr size_t INVALID_ID = 0;
    static size_t           GenerateID()
    {
        static std::atomic<size_t> current_id = 0;   // Start from 1 to avoid INVALID_ID
        current_id.fetch_add(1);
        return current_id;
    }
};


template <typename...>
class MultiCastDelegate;

template <typename R, typename... Args>
class MultiCastDelegate<R(Args...)>
{
public:
    using DelegateType       = Delegate<R(Args...)>;
    using HandleDelegatePair = std::pair<DelegateHandle, DelegateType>;

private:
    std::vector<HandleDelegatePair> delegate_array;

public:
    MultiCastDelegate() = default;

    MultiCastDelegate(MultiCastDelegate&&)            = default;
    MultiCastDelegate& operator=(MultiCastDelegate&&) = default;
    ~MultiCastDelegate()                              = default;

    MultiCastDelegate(MultiCastDelegate const&)            = delete;
    MultiCastDelegate& operator=(MultiCastDelegate const&) = delete;

    DelegateHandle Add(DelegateType const& handler)
    {
        delegate_array.emplace_back(DelegateHandle(0), handler);
        return delegate_array.back().first;
    }

    template <typename F>
    DelegateHandle AddLambda(F&& lambda)
    {
        return Add(DelegateType::CreateFromLambda(std::forward<F>(lambda)));
    }

    DelegateHandle AddStatic(R (*pf)(Args...))
    {
        ;
        return Add(DelegateType::CreateFromStatic(pf));
    }


    template <typename T>
    DelegateHandle AddMember(R (T::*mem_pfn)(Args...), T& instance)
    {
        return Add(DelegateType::CreateFromMember(mem_pfn, instance));
    }

    void RemoveAll() { delegate_array.clear(); }

    void Broadcast(Args... args)
    {
        for (size_t i = 0; i < delegate_array.size(); ++i) {
            if (delegate_array[i].first.IsValid()) delegate_array[i].second.ExecuteIfBound(std::forward<Args>(args)...);
        }
    }
};


struct EventArgs
{
    size_t flag = 0;
};

struct EventArgsInt : public EventArgs
{
    union
    {
        int data;
        int a;
        int x;
    };


    using base = EventArgs;
    EventArgsInt(int data)
        : base()
        , data(data)
    {}
};

struct EventArgsI2 : public EventArgs
{
    union
    {
        int data[2];
        struct
        {
            int a;
            int b;
        };
        struct
        {
            int x;
            int y;
        };
    };

    using base = EventArgs;
    EventArgsI2(int data1, int data2)
        : base()
        , data{data1, data2}
    {}
};

struct EventArgsFloat : public EventArgs
{
    union
    {
        float data;
        float a;
        float x;
    };
    using base = EventArgs;
    EventArgsFloat(float data)
        : base()
        , data(data)
    {}
};

struct EventArgsF2 : public EventArgs
{
    union
    {
        float data[2];
        struct
        {
            float a;
            float b;
        };
        struct
        {
            float x;
            float y;
        };
    };

    using base = EventArgs;
    EventArgsF2(float data1, float data2)
        : base()
        , data{data1, data2}
    {}
};

#define DECLARE_EVENT(EVENT_NAME, ...)                             \
    class EVENT_NAME : public MultiCastDelegate<void(__VA_ARGS__)> \
    {};
#define DECLARE_EVENT2(EVENT_NAME, ARGS_TYPE)                     \
    class EVENT_NAME : public MultiCastDelegate<void(ARGS_TYPE&)> \
    {};

}   // namespace SP