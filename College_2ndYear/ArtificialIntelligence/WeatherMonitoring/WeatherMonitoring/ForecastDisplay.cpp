
#include "ForecastDisplay.h"
#include <iostream>

ForecastDisplay::ForecastDisplay()
{
    memset(&mCurrentWeatherData, 0, sizeof(WeatherData));
}

void ForecastDisplay::Update(const WeatherData& parameter)
{
    mCurrentWeatherData = parameter;
}

void ForecastDisplay::Display()
{
    const float temperature = mCurrentWeatherData.Temperature;
    const float humidity = mCurrentWeatherData.Humidity;

    std::cout << "<Forecast Weather>" << std::endl;
    if (humidity > 60.f)
    {
        if (temperature > 0.f)
        {
            std::cout << "Rainy weather" << std::endl;
        }
        else
        {
            std::cout << "Snowy weather" << std::endl;
        }
    }
    else
    {
        if (temperature > 30.f)
        {
            std::cout << "Hot weather" << std::endl;
        }
        else if (temperature > 0.f)
        {
            std::cout << "Cool weather" << std::endl;
        }
        else
        {
            std::cout << "Freeze weather" << std::endl;
        }
    }
}
