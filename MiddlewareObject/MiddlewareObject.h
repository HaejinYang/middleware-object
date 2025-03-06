#pragma once

#include <functional>

template <typename T, typename R>
class MiddlewareObject 
{
    using MiddlewareFunc = std::function<R(T, std::function<R(T)>)>;

public:
    static MiddlewareObject<T, R> of(MiddlewareFunc func) 
    {
        return MiddlewareObject<T, R>(func);
    }

    static MiddlewareObject<T, R> empty() 
    {
        return MiddlewareObject<T, R>([](T value, std::function<R(T)> next) 
            {
                return next(value);
            });
    }

    MiddlewareObject<T, R> andThen(const MiddlewareObject<T, R>& after) const 
    {
        MiddlewareFunc currentMiddleware = middleware;
        MiddlewareFunc afterMiddleware = after.middleware;

        return MiddlewareObject<T, R>([=](T value, std::function<R(T)> next) 
            {
                return currentMiddleware(value, [&](T value2) 
                    {
                        return afterMiddleware(value2, next);
                    });
            });
    }

    MiddlewareObject<T, R> andThen(MiddlewareFunc func) const 
    {
        return andThen(MiddlewareObject<T, R>::of(func));
    }

    R run(T param, std::function<R(T)> task) const 
    {
        return middleware(param, task);
    }

private:
    MiddlewareFunc middleware;
    MiddlewareObject(MiddlewareFunc func) : middleware(func) {}
};