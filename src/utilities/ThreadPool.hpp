#pragma once

#include <queue>
#include <type_traits>

#include "ThreadsafeQueue.hpp"

#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>
#include <boost/thread/thread.hpp>

class IFunctionWrapper
{
public:

};

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

class TaskManager
{
    //using TaskType = FunctionWrapper;
    //using LocalQueueType = std::queue<TaskType>;
private:
    //std::atomic_bool done_;

    boost::asio::thread_pool pool_;

    //ThreadsafeQueue<TaskType> global_queue_;
    //std::vector<std::unique_ptr<StealingQueue>> queues_;

    //out
    //std::vector<std::thread> threads_;

    //MEMORY LEAK
    //inline static thread_local StealingQueue* local_queue_;

    //out
    //inline static thread_local size_t my_index_;
public:
    TaskManager(uint threads_count):
        pool_{threads_count}
    {}
    ~TaskManager()
    {
        pool_.join();
    }
    //typename std::result_of<Function>::type
    template<typename Function, typename ... Args>
    auto addTask(Function&& new_task, Args... args) -> std::future<decltype(new_task(args...))>
    {
        typedef decltype(new_task(args...)) ResultType;

        std::promise<ResultType> promise;

        auto res{promise.get_future()};

        boost::asio::post(pool_ , 
        [local_promise = std::move(promise) , 
         task_to_call  = std::forward<Function>(new_task) ,
         args... ] () mutable
        {
            local_promise.set_value(task_to_call(args...));
        });

        return res;
    }
};
