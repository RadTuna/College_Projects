#pragma once

#include "IObserver.h"
#include "NewspaperData.h"
#include "IDisplay.h"

class PCDisplay final : public IObserver<NewspaperData>, public IDisplay
{
public:
    explicit PCDisplay() = default;
    virtual ~PCDisplay() = default;

    void Update(const NewspaperData& parameter) override;
    void Display() override;

private:
    NewspaperData mNewspaperData;

};
