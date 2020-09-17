
#include "PCDisplay.h"
#include <iostream>

void PCDisplay::Update(const NewspaperData& parameter)
{
    mNewspaperData = parameter;
}

void PCDisplay::Display()
{
    std::cout << "<" << mNewspaperData.NewsName << ">" << std::endl;
    std::cout << "Created date : " << mNewspaperData.CreatedDate <<
        " / Author : " << mNewspaperData.Author << std::endl;
    std::cout << mNewspaperData.NewsBody << std::endl;
    std::cout << std::endl;
}
