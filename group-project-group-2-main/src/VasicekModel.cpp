#include "VasicekModel.hpp"
#include <iostream>
#include <vector>
#include <fstream>

// Simulate the next interest rate using Vasicek model
double VasicekModel::simulateNextRate(double currentRate, double timeStep){
    //time step parameter check 
    if(timeStep < 0){
        std::cerr << "Time step may not be negative." << std::endl;
        return 0;
    }

    // Generate normal random variable
    double dw = distribution(generator);

    // Calculate next rate using Vasicek model
    return currentRate + MeanReversion * (LongTermMean - currentRate) * timeStep 
            + Volatility * std::sqrt(timeStep) * dw;
}