#pragma once

#include "NewspaperData.h"
#include "Subject.h"

class Reporter final
{
public:
    explicit Reporter() = default;
    ~Reporter() = default;

    Reporter& SetNewspaperName(const std::string& newspaperName);
    Reporter& SetAuthor(const std::string& author);
    Reporter& SetCreatedDate(const std::string& createdDate);
    Reporter& SetNewspaperBody(const std::string& newspaperBody);

    const NewspaperData& GetNewspaperData() const;

private:
    NewspaperData mNewspaperData;

};
