#pragma once

#include "InterestRateModel.hpp"

// Class to implement CIR model
class CIRModel : public InterestRateModel{
    public:
      
        // Constructor for CIRModel class
        CIRModel(double MeanRev, double LTM, double Vol) : InterestRateModel(MeanRev, LTM, Vol){

        };

        // Simulate next interest rate using CIR model
        double simulateNextRate(double currentRate, double timeStep);
};

