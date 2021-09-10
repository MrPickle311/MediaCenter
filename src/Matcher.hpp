#pragma once

#include <QString>
#include <regex>

class IMatcher
{
public:
    virtual QString extractKey(const QString& command) = 0;
};

class Matcher : public IMatcher
{
protected:
    const std::regex matcher_body_;
public:
    Matcher(std::string regex_pattern);
    virtual QString extractKey(const QString& command);
};

class CommandMatcher : public Matcher
{
public:
    CommandMatcher();
};