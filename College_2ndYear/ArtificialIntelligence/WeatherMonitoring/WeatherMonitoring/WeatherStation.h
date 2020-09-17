#pragma once

#include <memory>
#include "WeatherMediator.h"
#include "WeatherData.h"

class WeatherStation final
{
public:
    WeatherStation();
    ~WeatherStation() = default;

    void SetMeasurements(const WeatherData& weatherData);

    float GetTemperature() const;
    float GetHumidity() const;
    float GetPressure() const;

    WeatherMediator<WeatherData>* GetWeatherMediator() const;

private:
    std::unique_ptr<WeatherMediator<WeatherData>> mWeatherMediator;
    WeatherData mWeatherData;

};
