#pragma once

#include <QJsonDocument>
#include <QString>
#include <regex>

class IMatcher
{
public:
    virtual QString extractKey(QJsonDocument& command) = 0;
};

class Matcher : public IMatcher
{
protected:
    const std::regex matcher_body_;
    int desired_parse_pos_;

public:
    Matcher(std::string regex_pattern, int desired_parse_pos);
    virtual QString extractKey(const QString& command);
    void setDesiredParsedPos(int desired_parse_pos);
};

class CommandMatcher : public Matcher
{
public:
    CommandMatcher(int desired_parser_pos);
};