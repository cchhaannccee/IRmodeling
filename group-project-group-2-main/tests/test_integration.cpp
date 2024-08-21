#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Bond.hpp"
#include "VasicekModel.hpp"
#include "CIRModel.hpp"
#include "RateSimulator.hpp"
#include "Swaption.hpp"
#include <vector>
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

// Integration test for the entire program
TEST_CASE("Integration Test: Bond and Swaption Pricing with Simulated Rates", "[Integration]") {
    // Set up the models and parameters
    VasicekModel vasicek(0.1, 0.05, 0.01);
    CIRModel cir(0.1, 0.05, 0.01);
    Bond bond(1000, 10, 0.01, 0.5);
    Swaption swaption(0.05, 5, 1000, 1);
    RateSimulator simulator;

	// Parameters
    double initialRate = 0.03;
    double timeStep = 0.05;
    unsigned int steps = 10 / timeStep;

    // Simulate interest rate paths
    std::vector<double> VasicekRates = simulator.simulatePaths(vasicek, initialRate, timeStep, steps);
    std::vector<double> CIRRates = simulator.simulatePaths(cir, initialRate, timeStep, steps);

    // Price the bond using simulated rates
    double VasicekBondPrice = bond.price(VasicekRates, timeStep);
    double CIRBondPrice = bond.price(CIRRates, timeStep);

    // Ensure bond prices are positive
    REQUIRE(VasicekBondPrice > 0);
    REQUIRE(CIRBondPrice > 0);

    // Price the swaption using the simulated rate paths
    double VasicekSwaptionPricePayer = swaption.price(VasicekRates, 0.2, timeStep, 4, true);
    double VasicekSwaptionPriceReceiver = swaption.price(VasicekRates, 0.2, timeStep, 4, false);
    double CIRSwaptionPricePayer = swaption.price(CIRRates, 0.2, timeStep, 4, true);
    double CIRSwaptionPriceReceiver = swaption.price(CIRRates, 0.2, timeStep, 4, false);

    // Ensure swaption prices are positive
    REQUIRE(VasicekSwaptionPricePayer > 0);
    REQUIRE(VasicekSwaptionPriceReceiver > 0);
    REQUIRE(CIRSwaptionPricePayer > 0);
    REQUIRE(CIRSwaptionPriceReceiver > 0);

    // Check swaption prices under different volatilities
    double VasicekSwaptionPricePayerHighVol = swaption.price(VasicekRates, 0.4, timeStep, 4, true);
    double VasicekSwaptionPriceReceiverHighVol = swaption.price(VasicekRates, 0.4, timeStep, 4, false);
    double CIRSwaptionPricePayerHighVol = swaption.price(CIRRates, 0.4, timeStep, 4, true);
    double CIRSwaptionPriceReceiverHighVol = swaption.price(CIRRates, 0.4, timeStep, 4, false);

    // Ensure that higher volatility increases swaption prices
    REQUIRE(VasicekSwaptionPricePayerHighVol >= VasicekSwaptionPricePayer);
    REQUIRE(VasicekSwaptionPriceReceiverHighVol >= VasicekSwaptionPriceReceiver);
    REQUIRE(CIRSwaptionPricePayerHighVol >= CIRSwaptionPricePayer);
    REQUIRE(CIRSwaptionPriceReceiverHighVol >= CIRSwaptionPriceReceiver);
}