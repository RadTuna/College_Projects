
#include "SmartphoneDisplay.h"
#include <iostream>


void SmartphoneDisplay::Update(const NewspaperData& parameter)
{
    mNewspaperData = parameter;
}

void SmartphoneDisplay::Display()
{
    std::cout << "[" << mNewspaperData.NewsName << "]" <<
        " / Created date : " << mNewspaperData.CreatedDate << std::endl;
    std::cout << std::endl;
}
