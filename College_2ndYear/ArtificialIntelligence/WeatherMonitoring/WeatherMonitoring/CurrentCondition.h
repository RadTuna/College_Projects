#pragma once

#include "IObserver.h"
#include "IDisplay.h"
#include "WeatherData.h"

class CurrentCondition final : public IObserver<WeatherData>, public IDisplay
{
public:
    CurrentCondition();
    virtual ~CurrentCondition() = default;

    void Update(const WeatherData& parameter) override;
    void Display() override;

private:
    WeatherData mCurrentWeatherData;

};