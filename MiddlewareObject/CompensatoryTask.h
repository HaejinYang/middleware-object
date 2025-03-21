#pragma once
#include <iostream>
#include <functional>
#include <vector>
#include <stack>

class CompensatoryTask 
{
public:
    using Task = std::function<bool()>; // 성공 시 true, 실패 시 false 반환
    using Compensation = std::function<void()>; // 실패 시 실행할 코드

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
        std::stack<Compensation> compensationStack; // 실패 시 실행할 보상 스택

        for (auto& [task, compensation] : tasks) 
        {
            if (!task()) 
            { // 태스크 실패 시
                compensationStack.push(compensation);
                break; // 실패하면 다음 태스크 실행 안 함
            }
        }

        // 보상 실행 (역순)
        while (!compensationStack.empty()) 
        {
            compensationStack.top()(); // 스택에서 꺼내서 실행
            compensationStack.pop();
        }
    }

private:
    std::vector<std::pair<Task, Compensation>> tasks; // 태스크 및 보상 저장
};