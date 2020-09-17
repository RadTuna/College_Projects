#pragma once

#include "IObserver.h"
#include "IDisplay.h"
#include "WeatherData.h"

class StaticsDisplay final : public IObserver<WeatherData>, public IDisplay
{
public:
    StaticsDisplay();
    virtual ~StaticsDisplay() = default;

    void Update(const WeatherData& parameter) override;
    void Display() override;

private:
    WeatherData mAverageWeatherData;
    float mAverageWeight;

    WeatherData mMaxWeatherData;
    WeatherData mMinWeatherData;

};
