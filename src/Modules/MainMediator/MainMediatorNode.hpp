#pragma once
#include <Mediator.hpp>
#include <SystemNode.hpp>

namespace modules
{

template <typename CreatedType, typename... ContructorArgsTypes>
class Dependency : public std::shared_ptr<CreatedType>
{
public:
    explicit Dependency(ContructorArgsTypes... args_types) :
        std::shared_ptr<CreatedType>{ std::make_shared<CreatedType>(
            std::forward(args_types...)) }
    {}
};

class MainMediatorMatcher : public common::IMatcher
{
public:
    QString extractKey(QJsonDocument& command) override;
};

class MainMediatorNode
{
private:
public:
    MainMediatorNode();

private:
    MainMediatorMatcher matcher_;
    std::shared_ptr<common::Mediator> med_;
    std::shared_ptr<common::SystemNodeEnvironment> env_;
    common::SystemNode node_;
};

} // namespace modules
