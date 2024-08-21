#pragma once

#include <vector>
#include "InterestRateModel.hpp"
#include "Bond.hpp"

// Class for simulating paths and pricing bonds
class RateSimulator{
    public:

        // Simulate interest rate paths
        std::vector<double> simulatePaths(InterestRateModel& model, double InitialRate,
                                            double timeStep, unsigned int steps) const;
        
        // Price a bond using paths
        double priceBond(const Bond& bond, InterestRateModel& model, double InitialRate,
                                            double timeStep, unsigned int steps) const;
};

