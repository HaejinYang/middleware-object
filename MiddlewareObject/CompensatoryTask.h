#pragma once
#include <iostream>
#include <functional>
#include <vector>
#include <stack>

class CompensatoryTask 
{
public:
    using Task = std::function<bool()>; // ���� �� true, ���� �� false ��ȯ
    using Compensation = std::function<void()>; // ���� �� ������ �ڵ�

    CompensatoryTask& of(Task task, Compensation compensation)
    {
        tasks.push_back({ task, compensation });

        return *this;
    }

    CompensatoryTask& of(Task task)
    {
        tasks.push_back({ task, []() {} });

        return *this;
    }

    CompensatoryTask& andThen(Task task, Compensation compensation)
    {
        tasks.push_back({ task, compensation });

        return *this;
    }

    CompensatoryTask& andThen(Task task) 
    {
        tasks.push_back({ task, []() {} });

        return *this;
    }

    void run() 
    {
        std::stack<Compensation> compensationStack; // ���� �� ������ ���� ����

        for (auto& [task, compensation] : tasks) 
        {
            if (!task()) 
            { // �½�ũ ���� ��
                compensationStack.push(compensation);
                break; // �����ϸ� ���� �½�ũ ���� �� ��
            }
        }

        // ���� ���� (����)
        while (!compensationStack.empty()) 
        {
            compensationStack.top()(); // ���ÿ��� ������ ����
            compensationStack.pop();
        }
    }

private:
    std::vector<std::pair<Task, Compensation>> tasks; // �½�ũ �� ���� ����
};