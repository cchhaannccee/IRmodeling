#include "CIRModel.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

// Simulate next interest rate with CIR Model
double CIRModel::simulateNextRate(double currentRate, double timeStep){

    // Parameter check
    if(timeStep < 0){
        std::cerr << "Time step may not be negative." << std::endl;
        return 0;
    }
    if(currentRate < 0){
        std::cerr << "Current rate must be non-negative." << std::endl;
        return 0;
    }

    // Generate normal random variable
    double dw = distribution(generator);

    // Calculate next rate using CIR model, ensuring non-negative rates
    double sqrtRate = std::sqrt(std::max(currentRate, 0.0));
    double nextRate = currentRate + MeanReversion * (LongTermMean - currentRate) * timeStep 
                      + Volatility * sqrtRate * std::sqrt(timeStep) * dw;

    // Ensure the next rate is non-negative
    nextRate = std::max(nextRate, 0.0);
    
    return nextRate;
}