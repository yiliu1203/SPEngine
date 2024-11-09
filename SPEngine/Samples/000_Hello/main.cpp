#include <iostream>

[[nodiscard("no-discard-reason")]] int test_discard(int a)
{
    return a;
}

int main()
{

    std::cout << "Hello World" << std::endl;
    auto c = test_discard(1);
    std::cout << c;
    return 0;
}