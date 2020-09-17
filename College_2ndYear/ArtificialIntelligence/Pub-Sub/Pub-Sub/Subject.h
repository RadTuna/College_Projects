#pragma once

#include <vector>
#include <cassert>
#include "IObserver.h"

template<typename Param>
class Subject final
{
public:
    explicit Subject() = default;
    ~Subject() = default;

    void Subscribe(class IObserver<Param>* observer); // same function registerObserver;
    void Unsubscribe(class IObserver<Param>* observer); // same function removeObserver;

    void Publish(const Param& parameter); // same function notify;

private:
    std::vector<IObserver<Param>*> mSubscribers;

};

template <typename Param>
void Subject<Param>::Subscribe(IObserver<Param>* observer)
{
    assert(observer != nullptr);

    mSubscribers.emplace_back(observer);
}

template <typename Param>
void Subject<Param>::Unsubscribe(IObserver<Param>* observer)
{
    assert(observer != nullptr);

    auto findIter = std::find(mSubscribers.begin(), mSubscribers.end(), observer);
    if (findIter != mSubscribers.end())
    {
        mSubscribers.erase(findIter);
    }
}

template <typename Param>
void Subject<Param>::Publish(const Param& parameter)
{
    for (IObserver<Param>* observer : mSubscribers)
    {
        observer->Update(parameter);
    }
}

