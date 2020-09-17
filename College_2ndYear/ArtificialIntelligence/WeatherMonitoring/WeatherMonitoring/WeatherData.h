#pragma once

struct WeatherData
{
    WeatherData() : Temperature(0.f), Humidity(0.f), Pressure(0.f) {}
    WeatherData(float temperature, float humidity, float pressure)
        : Temperature(temperature)
        , Humidity(humidity)
        , Pressure(pressure) {}

    float Temperature;
    float Humidity;
    float Pressure;
};
