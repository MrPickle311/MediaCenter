#include "Matcher.hpp"

Matcher::Matcher(std::string regex_pattern):
         matcher_body_{regex_pattern}
    {}

QString Matcher::extractKey(const QString& command)
{
    thread_local static std::string str{command.toStdString()};
    thread_local static std::smatch match_results;
    if(std::regex_search(str , match_results  , matcher_body_))
    {
        return QString::fromStdString(match_results[0].str());
    }
    return "WrongCmd";
}

CommandMatcher::CommandMatcher():
    Matcher{"[A-Z]{1}[a-z]+"}
{}