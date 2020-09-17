
#include "StaticsDisplay.h"
#include <iostream>
#include <limits>

StaticsDisplay::StaticsDisplay()
    : mAverageWeight(0.f)
{
    memset(&mAverageWeatherData, 0, sizeof(WeatherData));

    constexpr float minFloat = std::numeric_limits<float>::min();
    constexpr float maxFloat = std::numeric_limits<float>::max();

    mMaxWeatherData.Temperature = minFloat;
    mMaxWeatherData.Humidity = minFloat;
    mMaxWeatherData.Pressure = minFloat;

    mMinWeatherData.Temperature = maxFloat;
    mMinWeatherData.Humidity = maxFloat;
    mMinWeatherData.Pressure = maxFloat;
}

void StaticsDisplay::Update(const WeatherData& parameter)
{
    mAverageWeatherData.Temperature = 
        (mAverageWeatherData.Temperature * mAverageWeight + parameter.Temperature) / (mAverageWeight + 1.f);
    mAverageWeatherData.Humidity = 
        (mAverageWeatherData.Humidity * mAverageWeight + parameter.Humidity) / (mAverageWeight + 1.f);
    mAverageWeatherData.Pressure = 
        (mAverageWeatherData.Pressure * mAverageWeight + parameter.Pressure) / (mAverageWeight + 1.f);
    mAverageWeight += 1.f;

    mMaxWeatherData.Temperature = std::max(mMaxWeatherData.Temperature, parameter.Temperature);
    mMaxWeatherData.Humidity = std::max(mMaxWeatherData.Humidity, parameter.Humidity);
    mMaxWeatherData.Pressure = std::max(mMaxWeatherData.Pressure, parameter.Pressure);

    mMinWeatherData.Temperature = std::min(mMinWeatherData.Temperature, parameter.Temperature);
    mMinWeatherData.Humidity = std::min(mMinWeatherData.Humidity, parameter.Humidity);
    mMinWeatherData.Pressure = std::min(mMinWeatherData.Pressure, parameter.Pressure);
}

void StaticsDisplay::Display()
{
    std::cout << "<Statics Weather>" << std::endl;

    std::cout << "Average : " << mAverageWeatherData.Temperature << " / "<< 
        mAverageWeatherData.Humidity << " / " <<
        mAverageWeatherData.Pressure << std::endl;

    std::cout << "Max : " << mMaxWeatherData.Temperature << " / "<< 
        mMaxWeatherData.Humidity << " / " <<
        mMaxWeatherData.Pressure << std::endl;

    std::cout << "Min : " << mMinWeatherData.Temperature << " / "<< 
        mMinWeatherData.Humidity << " / " <<
        mMinWeatherData.Pressure << std::endl;
}
