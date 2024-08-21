#include "Swaption.hpp"
#include <cmath>
#include <iostream>

// Constructor for Swaption class
Swaption::Swaption(double sr, double mat, double notional, double swapLen)
    : StrikeRate(sr), Maturity(mat), Notional(notional), SwapLength(swapLen) {}

// Normal distribution function
double Swaption::normalCDF(double x) const {
    return 0.5 * erfc(-x * sqrt(0.5));
}

// PV of annuities function
double Swaption::calculatePVA(double strikeRate, double f, double expiryTime) const {
    return ((1 - pow((1 + strikeRate / f), -(f * expiryTime))) / strikeRate);
}

// Price the swaption using Black's formula with simulated interest rates
double Swaption::price(const std::vector<double>& rates, double volatility, double timeStep, double f, bool isPayer) const {
    double forwardSwapRate = 0.0;
    int steps = static_cast<int>(Maturity / timeStep);
    int swapSteps = static_cast<int>(SwapLength / timeStep);

    // Ensure we have enough rates data
    if (steps + swapSteps > static_cast<int>(rates.size())) {
        std::cerr << "Error: Insufficient rates data for pricing." << std::endl;
        return 0.0;
    }

    // Calculate forward swap rate at maturity using rates in swap length 
    for (int i = steps; i < steps + swapSteps; ++i) {
        forwardSwapRate += rates[i];
    }
    forwardSwapRate /= swapSteps;

    double d1 = (log(forwardSwapRate / StrikeRate) + 0.5 * pow(volatility, 2) * Maturity) /
                (volatility * sqrt(Maturity));
    double d2 = d1 - volatility * sqrt(Maturity);

    double annuityFactor = calculatePVA(StrikeRate, f, Maturity);

    double presentValue;
    if (isPayer) {
        presentValue = Notional * annuityFactor * (forwardSwapRate * normalCDF(d1) - StrikeRate * normalCDF(d2));
    } else {
        presentValue = Notional * annuityFactor * (StrikeRate * normalCDF(-d2) - forwardSwapRate * normalCDF(-d1));
    }

    return presentValue;
}