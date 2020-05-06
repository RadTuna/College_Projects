#pragma once 

class CCalc
{
public:

    CCalc() { }
    ~CCalc() {}

    static double sum(double a, double b) { return a + b; }
    static double div(double a, double b) { return a / b; }
    static double multi(double a, double b) { return a * b; }
    static double minus(double a, double b) { return a - b; }

};
