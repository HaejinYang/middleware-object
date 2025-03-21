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

TEST(CompensatoryTaskTest, CompensatoryTask_Task_1��_����)
{
    Widget w;
    CompensatoryTask task;
    task.of(
        std::bind(&Widget::StoreValueBiggerThan100, &w, 200)
    );

    task.run();

    EXPECT_EQ(200, w.mVal);
}

TEST(CompensatoryTaskTest, CompensatoryTask_Task_1��_����_1��_����)
{
    Widget w;
    CompensatoryTask task;
    task.of(
        std::bind(&Widget::StoreValueBiggerThan100, &w, 50),  // ���� (50 < 100)
        std::bind(&Widget::StoreValueBiggerThan100, &w, 200)  // ���� ����
    );

    task.run();

    EXPECT_EQ(200, w.mVal);
}

TEST(CompensatoryTaskTest, CompensatoryTask_Task_������_����)
{
    Widget w;
    CompensatoryTask task;
    task.of(
        std::bind(&Widget::StoreValueBiggerThan100, &w, 120) // ����
    ).andThen(
        std::bind(&Widget::StoreValueBiggerThan100, &w, 150) // ����
    ).andThen(
        std::bind(&Widget::StoreValueBiggerThan100, &w, 180) // ����
    );

    task.run();

    EXPECT_EQ(180, w.mVal);
}

TEST(CompensatoryTaskTest, CompensatoryTask_Task_�߰�_����)
{
    Widget w;
    CompensatoryTask task;
    task.of(
        std::bind(&Widget::StoreValueBiggerThan100, &w, 120) // ����
    ).andThen(
        std::bind(&Widget::StoreValueBiggerThan100, &w, 80),  // ���� (80 < 100)
        std::bind(&Widget::StoreValueBiggerThan100, &w, 130)  // ���� ����
    ).andThen(
        std::bind(&Widget::StoreValueBiggerThan100, &w, 150)  // ������� ����
    );

    task.run();

    EXPECT_EQ(130, w.mVal); // ���� �� ����(130)�� �����
}

TEST(CompensatoryTaskTest, CompensatoryTask_Task_����_���°��)
{
    Widget w;
    CompensatoryTask task;
    task.of(
        std::bind(&Widget::StoreValueBiggerThan100, &w, 50) // ���� (���� ����)
    );

    task.run();

    EXPECT_EQ(0, w.mVal); // ���� ������� ����
}

TEST(CompensatoryTaskTest, CompensatoryTask_Task_������_����_����_Ȯ��)
{
    Widget w;
    CompensatoryTask task;
    task.of(
        std::bind(&Widget::StoreValueBiggerThan100, &w, 120)  // ����
    ).andThen(
        std::bind(&Widget::StoreValueBiggerThan100, &w, 80),   // ����
        std::bind(&Widget::StoreValueBiggerThan100, &w, 130)   // ���� ����
    ).andThen(
        std::bind(&Widget::StoreValueBiggerThan100, &w, 150),  // ������� ����
        std::bind(&Widget::StoreValueBiggerThan100, &w, 200)   // ������� ����
    );

    task.run();

    EXPECT_EQ(130, w.mVal); // ������ �½�ũ�� ���� �����
}
