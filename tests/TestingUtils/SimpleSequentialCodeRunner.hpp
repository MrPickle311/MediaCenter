#pragma once

#include <functional>
#include <list>

namespace tests
{

class SimpleSequentialCodeRunner
{
public:
    using FunctionType = std::function<void()>;

private:
    std::list<FunctionType> fn_list_;

public:
    void addCodeToRun(FunctionType&& function)
    {
        fn_list_.push_back(std::move(function));
    }
    void runAll() const
    {
        for (auto&& fn : fn_list_)
        {
            fn();
        }
    }
};

} // namespace tests
