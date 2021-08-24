#pragma once
#include <queue>
#include "ThreadsafeQueue.hpp"

class IThreadJoiner
{
public:
    virtual ~IThreadJoiner(){}
};

class ThreadJoiner : public IThreadJoiner
{
private:
    std::vector<std::thread>* threads_;
public:
    explicit ThreadJoiner(std::vector<std::thread>* threads);
    virtual ~ThreadJoiner();
};

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

class ThreadPool
{
    using TaskType = FunctionWrapper;
    using LocalQueueType = std::queue<TaskType>;
private:
    std::atomic_bool done_;

    ThreadsafeQueue<TaskType> global_queue_;
    std::vector<std::unique_ptr<StealingQueue>> queues_;

    //out
    std::vector<std::thread> threads_;

    //out
    ThreadJoiner joiner_;

    inline static thread_local StealingQueue* local_queue_;

    //out
    inline static thread_local size_t my_index_;
private:
    void workerThread(size_t my_index);
    bool popTaskFromLocalQueue(TaskType& task);
    bool popTaskFromGlobalQueue(TaskType& task);
    bool popTaskFromOtherQueue(TaskType& task);
public:
    ThreadPool();
    virtual ~ThreadPool();
    void runPendingTask();
    template<typename FunctionType>
    std::future<typename std::result_of<FunctionType()>::type> submit(FunctionType new_task)
    {
        typedef typename std::result_of<FunctionType()>::type ResultType;

        std::packaged_task<ResultType()> task{std::move(new_task)};

        std::future<ResultType> res{task.get_future()};

        if(local_queue_)
        {
            local_queue_->push(std::move(task));
        } 
        else global_queue_.push(std::move(task));

        return res;
    }
};
