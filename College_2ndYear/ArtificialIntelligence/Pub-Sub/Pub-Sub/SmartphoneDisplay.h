#pragma once

#include "IObserver.h"
#include "NewspaperData.h"
#include "IDisplay.h"

class SmartphoneDisplay final : public IObserver<NewspaperData>, public IDisplay
{
public:
    explicit SmartphoneDisplay() = default;
    virtual ~SmartphoneDisplay() = default;

    void Update(const NewspaperData& parameter) override;
    void Display() override;

private:
    NewspaperData mNewspaperData;

};
