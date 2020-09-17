
#include "WeatherStation.h"


WeatherStation::WeatherStation()
{
    mWeatherMediator = std::make_unique<WeatherMediator<WeatherData>>();
    memset(&mWeatherData, 0, sizeof(WeatherData));
}

void WeatherStation::SetMeasurements(const WeatherData& weatherData)
{
    mWeatherData = weatherData;

    mWeatherMediator->Publish(mWeatherData);
}

float WeatherStation::GetTemperature() const
{
    return mWeatherData.Temperature;
}

float WeatherStation::GetHumidity() const
{
    return mWeatherData.Humidity;
}

float WeatherStation::GetPressure() const
{
    return mWeatherData.Pressure;
}

WeatherMediator<WeatherData>* WeatherStation::GetWeatherMediator() const
{
    return mWeatherMediator.get();
}
