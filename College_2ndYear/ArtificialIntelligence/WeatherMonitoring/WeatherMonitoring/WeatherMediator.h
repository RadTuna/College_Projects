#pragma once

#include <vector>
#include <cassert>
#include "IObserver.h"

template<typename Param>
class WeatherMediator final
{
public:
    WeatherMediator() = default;
    ~WeatherMediator() = default;

    void Subscribe(class IObserver<Param>* observer);
    void Unsubscribe(class IObserver<Param>* observer);

    void Publish(const Param& parameter);

private:
    std::vector<IObserver<Param>*> mObservers;

};

template <typename Param>
void WeatherMediator<Param>::Subscribe(IObserver<Param>* observer)
{
    assert(observer != nullptr);

    mObservers.emplace_back(observer);
}

template <typename Param>
void WeatherMediator<Param>::Unsubscribe(IObserver<Param>* observer)
{
    assert(observer != nullptr);

    auto findIter = std::find(mObservers.begin(), mObservers.end(), observer);
    if (findIter != mObservers.end())
    {
        mObservers.erase(findIter);
    }
}

template <typename Param>
void WeatherMediator<Param>::Publish(const Param& parameter)
{
    for (IObserver<Param>* observer : mObservers)
    {
        observer->Update(parameter);
    }
}

