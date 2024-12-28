
#include <iostream>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>



template <size_t N>
struct fixed_string
{
    char             data[N];
    constexpr size_t length() const { return N; }

    constexpr fixed_string(const char (&array)[N]) { std::copy(array, array + N, data); }
};

// template <size_t N>
// fixed_string(const char (&)[N]) -> fixed_string<N>;


template <typename C, typename R, typename... A>
struct MemberFuncDetail
{
    using ReturnType = R;
    explicit constexpr MemberFuncDetail(R (C::*func)(A...) const) {}
    explicit constexpr MemberFuncDetail(R (C::*func)(A...)) {}
};


template <typename T>
struct TypeInfo
{};

#define TYPE_BEGIN(TypeName)                                                               \
    template <>                                                                            \
    struct TypeInfo<TypeName>                                                              \
    {                                                                                      \
        using RawType                          = TypeName;                                 \
        using MetaType                         = TypeInfo;                                 \
        static constexpr size_t _index_offset_ = __COUNTER__ + 1;                          \
        template <fixed_string S>                                                          \
        struct MemberField                                                                 \
        {                                                                                  \
            static_assert(S.length() <= 1, "input string should be in member_filed_list"); \
        };                                                                                 \
        template <size_t N>                                                                \
        struct IndexMemberField                                                            \
        {};

#define TYPE_FIELD(FIELD)                                                       \
    template <>                                                                 \
    struct MemberField<#FIELD>                                                  \
    {                                                                           \
        static constexpr std::string_view FieldName{#FIELD};                    \
        static constexpr auto             pointer{&RawType::FIELD};             \
    };                                                                          \
    template <>                                                                 \
    struct IndexMemberField<__COUNTER__ - _index_offset_> : MemberField<#FIELD> \
    {};

#define TYPE_FUNC(FUNC_NAME)                                                             \
    template <>                                                                          \
    struct MemberField<#FUNC_NAME>                                                       \
    {                                                                                    \
        static constexpr std::string_view FieldName{#FUNC_NAME};                         \
        using return_type = decltype(MemberFuncDetail(&RawType::FUNC_NAME))::ReturnType; \
    };

#define TYPE_END                                                       \
    static constexpr size_t MemberCount{__COUNTER__ - _index_offset_}; \
    }                                                                  \
    ;


template <typename T, size_t... N>
void traval_index(const T& t, std::index_sequence<N...>)
{
    auto tmp = {(std::cout << TypeInfo<T>::template IndexMemberField<N>::FieldName << ":"
                           << &t->*(TypeInfo<T>::template IndexMemberField<N>::pointer) << std::endl,
                 0)...};
}

template <typename T>
void travel_type(const T& t)
{
    traval_index(t, std::make_index_sequence<TypeInfo<T>::MemberCount>{});
}

struct People
{
    std::string name;
    int         age;
    std::string get_name() const { return name; };
};

TYPE_BEGIN(People)
TYPE_FIELD(name)
TYPE_FIELD(age)
TYPE_FUNC(get_name)
TYPE_END


int main()
{

    //part 1
    People p{"Jim", 20};
    static_assert(TypeInfo<People>::MemberField<"name">::FieldName == "name");
    // static_assert( TypeInfo<People>::MemberField<"test">::FieldName == "test");	// static_assert failed: 'input string should be in member_filed_list'
    static_assert(std::is_same_v<TypeInfo<People>::MemberField<"get_name">::return_type, std::string>);

    //part 2
    travel_type(p);

    return 1;
}
