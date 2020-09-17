#pragma once

#include "IObserver.h"
#include "IDisplay.h"
#include "WeatherData.h"

class ForecastDisplay final : public IObserver<WeatherData>, public IDisplay
{
public:
    ForecastDisplay();
    virtual ~ForecastDisplay() = default;

    void Update(const WeatherData& parameter) override;
    void Display() override;

private:
    WeatherData mCurrentWeatherData;

};
