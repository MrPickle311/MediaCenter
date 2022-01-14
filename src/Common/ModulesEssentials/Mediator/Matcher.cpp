#include "Matcher.hpp"
#include <iostream>

namespace common{

Matcher::Matcher(std::string regex_pattern, int desired_parse_pos):
         matcher_body_{regex_pattern} , 
         desired_parse_pos_{desired_parse_pos}
{}

QString Matcher::extractKey(const QString& command)
{
    auto str{command.toStdString()};
    std::smatch match_results;

    auto commands_beg{std::sregex_iterator{str.begin(), str.end() , matcher_body_ }};

    if(std::distance(commands_beg ,std::sregex_iterator{} ) == 0)
    {
        return "WrongCmd";
    }

    for(auto i{0} ; i < this->desired_parse_pos_ ; ++i)
    {
        ++commands_beg;
    }

    std::cout << (*commands_beg)[0] << " " << desired_parse_pos_ << std::endl;

    return QString::fromStdString((*commands_beg)[0]);

    // if(std::regex_search(str , match_results  , matcher_body_ , std::regex_constants::match_continuous))
    // {
        // std::cout << match_results.size() << std::endl;
        // return QString::fromStdString(match_results[desired_parse_pos_].str());
    // }
// 
    // std::cout << match_results[desired_parse_pos_].str() << std::endl;
}

void Matcher::setDesiredParsedPos(int desired_parse_pos) 
{
    this->desired_parse_pos_ = desired_parse_pos;
}

CommandMatcher::CommandMatcher(int desired_parser_pos):
    Matcher{"[A-Z]{1}[a-z]+" , desired_parser_pos}
{}

}