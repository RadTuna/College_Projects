
#include "Reporter.h"


Reporter& Reporter::SetNewspaperName(const std::string& newspaperName)
{
    mNewspaperData.NewsName = newspaperName;
    return *this;
}

Reporter& Reporter::SetAuthor(const std::string& author)
{
    mNewspaperData.Author = author;
    return *this;
}

Reporter& Reporter::SetCreatedDate(const std::string& createdDate)
{
    mNewspaperData.CreatedDate = createdDate;
    return *this;
}

Reporter& Reporter::SetNewspaperBody(const std::string& newspaperBody)
{
    mNewspaperData.NewsBody = newspaperBody;
    return *this;
}

const NewspaperData& Reporter::GetNewspaperData() const
{
    return mNewspaperData;
}


