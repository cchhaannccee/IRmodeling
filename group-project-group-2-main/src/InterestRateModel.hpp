#pragma once
#include <cmath>
#include <random>

// Abstract base class for interest rate models
class InterestRateModel{
    protected:
        // Variables shared by derived classes
          // Create random number generator for normal distribution
        std::default_random_engine generator;
        std::normal_distribution<double> distribution;
        
        double MeanReversion;
        double LongTermMean;
        double Volatility;
    public:
        // Constructor for InterestRateModel class
        InterestRateModel(double MeanRev, double LTM, double Vol) : MeanReversion(MeanRev), LongTermMean(LTM), Volatility(Vol) {
            distribution = std::normal_distribution<double>(0.0,1.0);
        }

        // Pure virtual function to simulate next interest rate
        virtual double simulateNextRate(double currentRate, double timeStep) = 0;
};

