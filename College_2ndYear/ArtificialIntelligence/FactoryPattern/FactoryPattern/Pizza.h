#pragma once

#include <string>

class Pizza
{
public:
    Pizza() = default;
    virtual ~Pizza() = 0;

    void PrintDescription() const;

protected:
    std::string mDescription;

};

inline Pizza::~Pizza() = default;

