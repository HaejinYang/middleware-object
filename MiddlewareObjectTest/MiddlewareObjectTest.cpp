#include "pch.h"
#include "../MiddlewareObject/MiddlewareObject.h"

TEST(MiddlewareObjectTest, Basic_Middleware_Functionality) 
{
    MiddlewareObject<int, int> middleware = MiddlewareObject<int, int>::of
    (
        [](int value, std::function<int(int)> next) 
        {
            return next(value + 1);
        }
    );

    int result = middleware.run(5, [](int v) { return v * 2; });
    EXPECT_EQ(result, 12); // (5 + 1) * 2 = 12
}

TEST(MiddlewareObjectTest, Chained_Middleware) 
{
    auto addOne = MiddlewareObject<int, int>::of
    (
        [](int value, std::function<int(int)> next) 
        {
            return next(value + 1);
        }
    );

    auto multiplyByThree = MiddlewareObject<int, int>::of
    (
        [](int value, std::function<int(int)> next) 
        {
            return next(value * 3);
        }
    );

    auto middlewareChain = addOne.andThen(multiplyByThree);

    int result = middlewareChain.run(2, [](int v) { return v; });
    EXPECT_EQ(result, 9); // (2 + 1) * 3 = 9
}

TEST(MiddlewareObjectTest, Empty_Middleware) 
{
    auto emptyMiddleware = MiddlewareObject<int, int>::empty();
    int result = emptyMiddleware.run(10, [](int v) { return v * 2; });
    EXPECT_EQ(result, 20); // 그대로 실행됨 (10 * 2 = 20)
}

TEST(MiddlewareObjectTest, Multiple_Chaining) 
{
    auto addOne = MiddlewareObject<int, int>::of
    (
        [](int value, std::function<int(int)> next) 
        {
            return next(value + 1);
        }
    );

    auto multiplyByTwo = MiddlewareObject<int, int>::of
    (
        [](int value, std::function<int(int)> next) 
        {
            return next(value * 2);
        }
    );

    auto subtractThree = MiddlewareObject<int, int>::of
    (
        [](int value, std::function<int(int)> next) 
        {
            return next(value - 3);
        }
    );

    auto middlewareChain = addOne.andThen(multiplyByTwo).andThen(subtractThree);

    int result = middlewareChain.run(4, [](int v) { return v; });
    EXPECT_EQ(result, 7); // (((4 + 1) * 2) - 3) = 7
}

TEST(MiddlewareObjectTest, String_Middleware) 
{
    auto toUpperCase = MiddlewareObject<std::string, std::string>::of
    (
        [](std::string value, std::function<std::string(std::string)> next)
        {
            for (char& c : value) c = toupper(c);
            return next(value);
        }
    );

    auto addExclamation = MiddlewareObject<std::string, std::string>::of
    (
        [](std::string value, std::function<std::string(std::string)> next) 
        {
            return next(value + "!!!");
        }
    );

    auto middlewareChain = toUpperCase.andThen(addExclamation);
    std::string result = middlewareChain.run("hello", [](std::string s) { return s; });
    EXPECT_EQ(result, "HELLO!!!");
}