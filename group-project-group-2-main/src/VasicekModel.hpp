#pragma once

#include "InterestRateModel.hpp"

// Class to implement Vasicek interest rate model
class VasicekModel : public InterestRateModel{
    public:
        // Constructor for VasicekModel class
        VasicekModel(double MeanRev, double LTM, double Vol) : InterestRateModel(MeanRev, LTM, Vol){

        };

        // Simulate next interest rate using Vasicek model
        double simulateNextRate(double currentRate, double timeStep);
};

