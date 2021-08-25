#pragma once

#include <queue>
#include <type_traits>

#include "ThreadsafeQueue.hpp"

#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>
#include <boost/thread/thread.hpp>

//@brief A function object with moving semantics
class FunctionWrapper
{
private:
    struct IImplementation
    {
        virtual void call() = 0;
        virtual ~IImplementation() {}
    };

    std::unique_ptr<IImplementation> implementation_;

    template<typename PackagedTaskType>
    struct ImplementationType : public IImplementation
    {
        PackagedTaskType func_;
        ImplementationType(PackagedTaskType&& func_):
            func_{std::move(func_)}
        {}
        void call()
        {
            func_();
        }
    };
public:
    FunctionWrapper() = default;

    template<typename F>
    FunctionWrapper(F&& f):
        implementation_{new ImplementationType{std::move(f)}}
    {}

    FunctionWrapper(FunctionWrapper&& wrapper);

    void operator() ();
    FunctionWrapper& operator=(FunctionWrapper&& wrapper);

    FunctionWrapper(const FunctionWrapper&) = delete;
    FunctionWrapper(FunctionWrapper&) = delete;
    FunctionWrapper& operator=(const FunctionWrapper&) = delete;
};

class IStealingQueue
{
    using DataType = FunctionWrapper;
public:
    virtual void push(DataType data) = 0;
    virtual bool empty() const = 0;
    virtual bool tryPop(DataType& data) = 0;
    virtual bool trySteal(DataType& data) = 0;
};

class StealingQueue 
{
    using DataType = FunctionWrapper;
private:
    std::deque<DataType> tasks_;
    mutable std::mutex mutex_;
public:
    StealingQueue();
    StealingQueue(const StealingQueue&) = delete;
    StealingQueue& operator=(const StealingQueue&) = delete;

    void push(DataType data);
    bool empty() const;
    bool tryPop(DataType& data);
    bool trySteal(DataType& data);
};

// @brief Object of this class has a thread pool to which
// tasks can be added. User has the guarance that each 
// task is performed. 
class TaskManager
{
private:
    boost::asio::thread_pool pool_;
public:
    // @brief Constructor takes the number of using threads
    TaskManager(uint threads_count):
        pool_{threads_count}
    {}
    // @brief Finishes all tasks and destroys object  
    ~TaskManager()
    {
        pool_.join();
    }
    // @brief   This method adds a new task to the thread pool
    // @param   Function object to call
    // @param   Arguments its invokation
    // @return  Return future associated with task's result
    template<typename Function, typename ... Args>
    auto addTask(Function&& new_task, Args... args) -> std::future<decltype(new_task(args...))>
    {
        using ResultType = decltype(new_task(args...));// new_task return type

        std::promise<ResultType> promise;
        auto res{promise.get_future()}; // extern future binding for a user

        //forward task and its arguments to the thread pool
        boost::asio::post(pool_ , 
        [   //parameters in lambda
            local_promise = std::move(promise) , 
            task_to_call  = std::forward<Function>(new_task) , //perfect forward the task
            args... // function args
        ] () mutable
        {
            //if return type is void. Cannot assign void to variable
            if constexpr (std::is_same<ResultType,void>::value)
            {
                task_to_call(args...);
                local_promise.set_value();
            }
            else local_promise.set_value(task_to_call(args...));
        });

        return res; 
    }
};
