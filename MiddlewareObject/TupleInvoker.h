#pragma once
#include <tuple>
#include <utility>

template <class ObjType, class Func, class... TupleArgs>
auto InvokeWithTuple(ObjType* obj, Func memfunc, const std::tuple<TupleArgs...>& targ)
{
    return std::apply([obj, memfunc](auto&&... args) -> decltype(auto)
        {
            return (obj->*memfunc)(std::forward<decltype(args)>(args)...);
        }, targ);
}