#include "RateSimulator.hpp"

// Simulate interest rate paths
std::vector<double> RateSimulator::simulatePaths(InterestRateModel& model, double InitialRate,
                                            double timeStep, unsigned int steps) const{

    // Initialize a vector to store simulated rates
    std::vector<double> rates(steps);

    // Set initial rate
    double currentRate = InitialRate;

    // Simulate rates for number of steps
    for (int i = 0; i < steps; ++i){
        currentRate = model.simulateNextRate(currentRate, timeStep);
        rates[i] = currentRate;
    }
    return rates; 
}

// Price a bond using simulated interest rate paths
double RateSimulator::priceBond(const Bond& bond, InterestRateModel& model, double InitialRate,
                                            double timeStep, unsigned int steps) const{

    // Simulate interest rate paths
    std::vector<double> rates = simulatePaths(model, InitialRate, timeStep, steps);

    // Calculate bond price using simulated rate path
    return bond.price(rates, timeStep);
}