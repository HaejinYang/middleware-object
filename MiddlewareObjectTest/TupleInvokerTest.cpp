#include "pch.h"
#include "../MiddlewareObject/TupleInvoker.h"

class Widget
{
public:
    // 1. 인자가 없는 함수
    int NoArgsFunction()
    {
        return 42;
    }

    // 2. 인자가 1개인 함수
    void StoreValue(int a)
    {
        mSum = a;
    }

    // 3. 다양한 타입을 받는 함수
    std::string ConcatString(std::string a, std::string b)
    {
        return a + b;
    }

    double Multiply(double a, double b, double c)
    {
        return a * b * c;
    }

    // 4. const 함수
    int GetSumConst(int a, int b) const
    {
        return a + b;
    }

    int mSum;
};

TEST(TupleInvokerTest, Invoke_Tuple_With_ReturnValue)
{
    Widget w;

    int sum = InvokeWithTuple(&w, &Widget::GetSumConst, std::make_tuple(10, 20));

    EXPECT_EQ(30, sum);
}

TEST(TupleInvokerTest, Invoke_Tuple_NoArgs)
{
    Widget w;

    int result = InvokeWithTuple(&w, &Widget::NoArgsFunction, std::make_tuple());

    EXPECT_EQ(42, result);
}

TEST(TupleInvokerTest, Invoke_Tuple_SingleArg)
{
    Widget w;

    InvokeWithTuple(&w, &Widget::StoreValue, std::make_tuple(77));

    EXPECT_EQ(77, w.mSum);
}

TEST(TupleInvokerTest, Invoke_Tuple_MixedTypes)
{
    Widget w;

    std::string result = InvokeWithTuple(&w, &Widget::ConcatString, std::make_tuple(std::string("Hello, "), std::string("World!")));

    EXPECT_EQ("Hello, World!", result);
}

TEST(TupleInvokerTest, Invoke_Tuple_MultipleArgs)
{
    Widget w;

    double result = InvokeWithTuple(&w, &Widget::Multiply, std::make_tuple(1.5, 2.0, 3.0));

    EXPECT_DOUBLE_EQ(9.0, result);
}

TEST(TupleInvokerTest, Invoke_Tuple_With_ConstMethod)
{
    Widget w;

    int sum = InvokeWithTuple(&w, &Widget::GetSumConst, std::make_tuple(5, 15));

    EXPECT_EQ(20, sum);
}