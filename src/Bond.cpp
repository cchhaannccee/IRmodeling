#include "Bond.hpp"
#include <cmath>
#include <vector>
#include <iostream>

// Constructor for Bond class
Bond::Bond(double FV, double Mat, double CR, double f) : FaceValue(FV), Maturity(Mat), couponRate(CR), Frequency(f) {}

// Calculate the bond price
double Bond::price(const std::vector<double>& rates, double timeStep) const {
    double presentValue = 0.0;
    double cashFlow = FaceValue * couponRate * Frequency;
    int couponPeriods = static_cast<int>(Maturity / Frequency);
    int maxIndex = rates.size() - 1; // Maximum valid index for rates vector

    for (int i = 0; i < couponPeriods; ++i) {
        double time = (i + 1) * Frequency;
        int rateIndex = static_cast<int>(time / timeStep);

        // Use the last available rate if rateIndex is out of bounds
        if (rateIndex > maxIndex) {
            rateIndex = maxIndex; 
        }

        presentValue += cashFlow * std::exp(-rates[rateIndex] * time);
    }

    // Add present value of face value at maturity
    double maturityTime = Maturity;
    int maturityIndex = static_cast<int>(maturityTime / timeStep);

    if (maturityIndex > maxIndex) {
        maturityIndex = maxIndex; // Use the last available rate if maturityIndex is out of bounds
    }

    presentValue += FaceValue * std::exp(-rates[maturityIndex] * maturityTime);

    return presentValue;
}