#include "pch.h"
#include "../MiddlewareObject/CompensatoryTask.h"

class Widget
{
public:
    Widget() : mVal(0) {}

    bool StoreValueBiggerThan100(int val)
    {
        if (val > 100)
        {
            this->mVal = val;
            return true;
        }
        else
        {
            return false;
        }
    }

    void ResetValue()
    {
        mVal = 0;
    }

    int mVal;
};

TEST(CompensatoryTaskTest, CompensatoryTask_Task_1개_실행)
{
    Widget w;
    CompensatoryTask task;
    task.of(
        std::bind(&Widget::StoreValueBiggerThan100, &w, 200)
    );

    task.run();

    EXPECT_EQ(200, w.mVal);
}

TEST(CompensatoryTaskTest, CompensatoryTask_Task_1개_보상_1개_실행)
{
    Widget w;
    CompensatoryTask task;
    task.of(
        std::bind(&Widget::StoreValueBiggerThan100, &w, 50),  // 실패 (50 < 100)
        std::bind(&Widget::StoreValueBiggerThan100, &w, 200)  // 보상 실행
    );

    task.run();

    EXPECT_EQ(200, w.mVal);
}

TEST(CompensatoryTaskTest, CompensatoryTask_Task_여러개_성공)
{
    Widget w;
    CompensatoryTask task;
    task.of(
        std::bind(&Widget::StoreValueBiggerThan100, &w, 120) // 성공
    ).andThen(
        std::bind(&Widget::StoreValueBiggerThan100, &w, 150) // 성공
    ).andThen(
        std::bind(&Widget::StoreValueBiggerThan100, &w, 180) // 성공
    );

    task.run();

    EXPECT_EQ(180, w.mVal);
}

TEST(CompensatoryTaskTest, CompensatoryTask_Task_중간_실패)
{
    Widget w;
    CompensatoryTask task;
    task.of(
        std::bind(&Widget::StoreValueBiggerThan100, &w, 120) // 성공
    ).andThen(
        std::bind(&Widget::StoreValueBiggerThan100, &w, 80),  // 실패 (80 < 100)
        std::bind(&Widget::StoreValueBiggerThan100, &w, 130)  // 보상 실행
    ).andThen(
        std::bind(&Widget::StoreValueBiggerThan100, &w, 150)  // 실행되지 않음
    );

    task.run();

    EXPECT_EQ(130, w.mVal); // 실패 시 보상(130)이 실행됨
}

TEST(CompensatoryTaskTest, CompensatoryTask_Task_보상_없는경우)
{
    Widget w;
    CompensatoryTask task;
    task.of(
        std::bind(&Widget::StoreValueBiggerThan100, &w, 50) // 실패 (보상 없음)
    );

    task.run();

    EXPECT_EQ(0, w.mVal); // 값이 변경되지 않음
}

TEST(CompensatoryTaskTest, CompensatoryTask_Task_여러개_실패_보상_확인)
{
    Widget w;
    CompensatoryTask task;
    task.of(
        std::bind(&Widget::StoreValueBiggerThan100, &w, 120)  // 성공
    ).andThen(
        std::bind(&Widget::StoreValueBiggerThan100, &w, 80),   // 실패
        std::bind(&Widget::StoreValueBiggerThan100, &w, 130)   // 보상 실행
    ).andThen(
        std::bind(&Widget::StoreValueBiggerThan100, &w, 150),  // 실행되지 않음
        std::bind(&Widget::StoreValueBiggerThan100, &w, 200)   // 실행되지 않음
    );

    task.run();

    EXPECT_EQ(130, w.mVal); // 실패한 태스크의 보상만 실행됨
}
