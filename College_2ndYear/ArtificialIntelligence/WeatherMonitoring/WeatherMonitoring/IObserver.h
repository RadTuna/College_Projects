#pragma once

template<typename Param>
class IObserver
{
public:
    IObserver() = default;
    virtual ~IObserver() = default;

    virtual void Update(const Param& parameter) = 0;
};
