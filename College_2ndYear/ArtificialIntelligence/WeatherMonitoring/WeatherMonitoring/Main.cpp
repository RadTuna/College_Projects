
#include <memory>
#include <iostream>

#include "WeatherStation.h"
#include "CurrentCondition.h"
#include "ForecastDisplay.h"
#include "StaticsDisplay.h"
#include "WeatherData.h"

int main()
{
    std::unique_ptr<WeatherStation> weatherStation = std::make_unique<WeatherStation>();

    std::unique_ptr<CurrentCondition> currentCondition = std::make_unique<CurrentCondition>();
    std::unique_ptr<ForecastDisplay> forecastDisplay = std::make_unique<ForecastDisplay>();
    std::unique_ptr<StaticsDisplay> staticsDisplay = std::make_unique<StaticsDisplay>();

    weatherStation->GetWeatherMediator()->Subscribe(currentCondition.get());
    weatherStation->GetWeatherMediator()->Subscribe(forecastDisplay.get());
    weatherStation->GetWeatherMediator()->Subscribe(staticsDisplay.get());

    weatherStation->SetMeasurements(WeatherData(40.f, 20.f, 50.f));
    currentCondition->Display();
    forecastDisplay->Display();
    staticsDisplay->Display();

    std::cout << std::endl;

    weatherStation->SetMeasurements(WeatherData(20.f, 90.f, 20.f));
    currentCondition->Display();
    forecastDisplay->Display();
    staticsDisplay->Display();

    std::cout << std::endl;

    weatherStation->SetMeasurements(WeatherData(-5.f, 80.f, 20.f));
    currentCondition->Display();
    forecastDisplay->Display();
    staticsDisplay->Display();

    return 0;
}
