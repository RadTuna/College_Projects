
#include "CurrentCondition.h"
#include <iostream>

CurrentCondition::CurrentCondition()
{
    memset(&mCurrentWeatherData, 0, sizeof(WeatherData));
}

void CurrentCondition::Update(const WeatherData& parameter)
{
    mCurrentWeatherData = parameter;
}

void CurrentCondition::Display()
{
    std::cout << "<Current Weather Condition>" << std::endl;
    std::cout << "Temperature : " << mCurrentWeatherData.Temperature <<
        " / Humidity : " << mCurrentWeatherData.Humidity <<
        " / Pressure : " << mCurrentWeatherData.Pressure << std::endl;
}
