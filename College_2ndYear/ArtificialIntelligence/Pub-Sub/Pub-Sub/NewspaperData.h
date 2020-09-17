#pragma once

#include <string>

struct NewspaperData
{
    NewspaperData() : NewsName(""), Author(""), CreatedDate(""), NewsBody("") {}
    NewspaperData (const std::string& newsName, const std::string& author, 
        const std::string& createdData, const std::string& newsBody)
        : NewsName(newsBody)
        , Author (author)
        , CreatedDate(createdData)
        , NewsBody(newsBody) {}

    std::string NewsName;
    std::string Author;
    std::string CreatedDate;
    std::string NewsBody;
};
