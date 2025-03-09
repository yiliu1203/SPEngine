#pragma once
#include "PCH.h"

namespace SP {
template <typename T>
class Singleton
{
public:
    NoCopyable(Singleton)

        static T& Get()
    {
        if (Instance != nullptr) {
            return *Instance;
        }
        Instance = new T();
        return *Instance;
    };
    static void Reset()
    {
        SP_ASSERT(Instance != nullptr, "Attempting to destroy a singleton before it has been instantiated");
        delete Instance;
        Instance = nullptr;
    };

protected:
    Singleton() {};

    ~Singleton()
    {
        if (Instance != nullptr) {
            Reset();
        }
    };

protected:
    static T* Instance;
};

template <typename T>
T* Singleton<T>::Instance = nullptr;
}   // namespace SP