#include <iostream>
#include "MiddlewareObject.h"

MiddlewareObject<int, int> makeMiddleware() 
{
    MiddlewareObject<int, int> mw = MiddlewareObject<int, int>::of([](int value, std::function<int(int)> next) 
        {
            return next(value * 2);
        });

    return mw.andThen(MiddlewareObject<int, int>::of([](int value, std::function<int(int)> next) 
        {
            return next(value + 10);
        }));
}

int main()
{
    auto pipeline = makeMiddleware(); // ���⼭ ������ mw�� ����� (temporary object)

    int result = pipeline.run(5, [](int v) { return v; });

    std::cout << "Result: " << result << std::endl;
}