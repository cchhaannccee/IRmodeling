#pragma once

#include <vector>

// Class for interest rate swaps
class Swaption {
private:
    double StrikeRate;
    double Maturity;
    double Notional;
    double SwapLength;

    // Helper functions
    double normalCDF(double x) const;
    double calculatePVA( double strikeRate, double f, double expiryTime) const;

public:

    // Constructor for Swaption class
    Swaption(double StrikeRate, double Maturity, double Notional, double SwapLength);

    // Calculate the price of the swaption using Black's formula
    double price(const std::vector<double>& rates, double volatility, double timeStep, double f, bool isPayer) const;

};

