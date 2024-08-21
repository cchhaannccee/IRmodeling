#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "VasicekModel.hpp"
#include "CIRModel.hpp"
#include "RateSimulator.hpp"

#include <cmath>
#include <numeric> 

// Helper function to generate rate paths
std::vector<std::vector<double>> generateRatePaths(InterestRateModel& model, double initialRate, double timeStep, unsigned int steps, unsigned int numPaths) {
    RateSimulator simulator;
    std::vector<std::vector<double>> allPaths(numPaths);

    for (unsigned int i = 0; i < numPaths; ++i) {
        allPaths[i] = simulator.simulatePaths(model, initialRate, timeStep, steps);
    }

    return allPaths;
}


// Vasicek Model Tests
TEST_CASE("Vasicek Model mean reversion", "[VasicekModel]") {
    double meanRev = 0.5; 
    double longTermMean = 0.05;
    double vol = 0.01;
    double initialRate = 0.03;
    double timeStep = 0.01;
    unsigned int steps = 5000; 
    unsigned int numPaths = 1000; 

    VasicekModel model(meanRev, longTermMean, vol);
    auto allPaths = generateRatePaths(model, initialRate, timeStep, steps, numPaths);

    // Calculate the average rate at each step across all paths
    std::vector<double> averageRates(steps, 0.0);
    for (const auto& path : allPaths) {
        for (unsigned int i = 0; i < steps; ++i) {
            averageRates[i] += path[i];
        }
    }

    for (double& rate : averageRates) {
        rate /= numPaths;
    }

    double finalAverageRate = std::accumulate(averageRates.begin(), averageRates.end(), 0.0) / steps;
    REQUIRE(finalAverageRate == Approx(longTermMean).epsilon(0.05));
}

TEST_CASE("Vasicek Model volatility effect", "[VasicekModel]") {
    double meanRev = 0.2; 
    double longTermMean = 0.05;
    double vol = 0.2;
    double initialRate = 0.03;
    double timeStep = 0.01;
    unsigned int steps = 5000; 
    unsigned int numPaths = 1000; 

    VasicekModel model(meanRev, longTermMean, vol);
    auto allPaths = generateRatePaths(model, initialRate, timeStep, steps, numPaths);

    // Calculate the variance at each step across all paths
    std::vector<double> allRates;
    for (const auto& path : allPaths) {
        allRates.insert(allRates.end(), path.begin(), path.end());
    }

    double meanRate = std::accumulate(allRates.begin(), allRates.end(), 0.0) / allRates.size();
    double variance = 0.0;
    for (const double rate : allRates) {
        variance += (rate - meanRate) * (rate - meanRate);
    }
    variance /= allRates.size();
    REQUIRE(variance > 0.01); 
}

// CIR Model Tests
TEST_CASE("CIR Model mean reversion", "[CIRModel]") {
    double meanRev = 0.5; 
    double longTermMean = 0.05;
    double vol = 0.01;
    double initialRate = 0.03;
    double timeStep = 0.01;
    unsigned int steps = 5000; 
    unsigned int numPaths = 1000; 

    CIRModel model(meanRev, longTermMean, vol);
    auto allPaths = generateRatePaths(model, initialRate, timeStep, steps, numPaths);

    // Calculate the average rate at each step across all paths
    std::vector<double> averageRates(steps, 0.0);
    for (const auto& path : allPaths) {
        for (unsigned int i = 0; i < steps; ++i) {
            averageRates[i] += path[i];
        }
    }

    for (double& rate : averageRates) {
        rate /= numPaths;
    }

    double finalAverageRate = std::accumulate(averageRates.begin(), averageRates.end(), 0.0) / steps;
    REQUIRE(finalAverageRate == Approx(longTermMean).epsilon(0.05));
}

TEST_CASE("CIR Model volatility effect", "[CIRModel]") {
    double meanRev = 0.2; 
    double longTermMean = 0.05;
    double vol = 0.3;
    double initialRate = 0.03;
    double timeStep = 0.01;
    unsigned int steps = 5000; 
    unsigned int numPaths = 1000; 

    CIRModel model(meanRev, longTermMean, vol);
    auto allPaths = generateRatePaths(model, initialRate, timeStep, steps, numPaths);

    // Calculate the variance at each step across all paths
    std::vector<double> allRates;
    for (const auto& path : allPaths) {
        allRates.insert(allRates.end(), path.begin(), path.end());
    }

    double meanRate = std::accumulate(allRates.begin(), allRates.end(), 0.0) / allRates.size();
    double variance = 0.0;
    for (const double rate : allRates) {
        variance += (rate - meanRate) * (rate - meanRate);
    }
    variance /= allRates.size();
    std::cout << "Variance for CIR: " << variance << std::endl;
    REQUIRE(variance > 0.01); 
}