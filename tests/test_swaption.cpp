#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "Swaption.hpp"
#include "InterestRateModel.hpp"
#include "RateSimulator.hpp"
#include "VasicekModel.hpp"

// Helper function to generate constant rates
std::vector<double> generateConstantRates(double rate, unsigned int steps) {
    std::vector<double> rates(steps, rate);
    return rates;
}

// Helper function to calculate expected swaption price using Black's formula
double calculateExpectedSwaptionPrice(bool isPayer, double forwardSwapRate, double strikeRate,
                                      double volatility, double f, double expiryTime, double notional) {
    double d1 = (log(forwardSwapRate / strikeRate) + 0.5 * pow(volatility, 2) * expiryTime) /
                (volatility * sqrt(expiryTime));
    double d2 = d1 - volatility * sqrt(expiryTime);

    double PVA = (1 - pow((1 + strikeRate / f), -(f * expiryTime))) / strikeRate;

    if (isPayer) {
        return notional * PVA * (forwardSwapRate * 0.5 * erfc(-d1 * sqrt(0.5)) - strikeRate * 0.5 * erfc(-d2 * sqrt(0.5)));
    } else {
        return notional * PVA * (strikeRate * 0.5 * erfc(d2 * sqrt(0.5)) - forwardSwapRate * 0.5 * erfc(d1 * sqrt(0.5)));
    }
}

// Test Swaption Pricing with Constant Rates
TEST_CASE("Swaption Pricing with Constant Rates", "[Swaption]") {
    double strikeRate = 0.05;
    double maturity = 5.0;
    double notional = 1000.0;
    double swapLength = 1.0;
    double forwardSwapRate = 0.05;
    double volatility = 0.2;
    double timeStep = 0.01;
    bool isPayer = true;

    // Calculate the necessary length for rates vector
    unsigned int requiredSteps = static_cast<unsigned int>((maturity + swapLength) / timeStep);
    // Generate constant rates
    std::vector<double> constantRates = generateConstantRates(forwardSwapRate, requiredSteps);

    // Create Swaption
    Swaption swaption(strikeRate, maturity, notional, swapLength);

    // Calculate Swaption price using the Swaption class
    double swaptionPrice = swaption.price(constantRates, volatility, timeStep, 4, isPayer);

    // Calculate expected price using the helper function
    double expectedPrice = calculateExpectedSwaptionPrice(isPayer, forwardSwapRate, strikeRate, volatility, 4, maturity, notional);

    // Verify the price
    REQUIRE(swaptionPrice == Approx(expectedPrice).epsilon(0.01));
}

// Test Payer and Receiver Swaption
TEST_CASE("Payer and Receiver Swaption Pricing", "[Swaption]") {
    double strikeRate = 0.05;
    double maturity = 5.0;
    double notional = 1000.0;
    double swapLength = 1.0;
    double forwardSwapRate = 0.05;
    double volatility = 0.2;
    double timeStep = 0.01;

    // Calculate the necessary length for rates vector
    unsigned int requiredSteps = static_cast<unsigned int>((maturity + swapLength) / timeStep);

    // Generate constant rates
    std::vector<double> constantRates = generateConstantRates(forwardSwapRate, requiredSteps);

    // Create Swaption
    Swaption swaption(strikeRate, maturity, notional, swapLength);

    // Calculate Payer Swaption price
    double payerSwaptionPrice = swaption.price(constantRates, volatility, timeStep, 4, true);

    // Calculate Receiver Swaption price
    double receiverSwaptionPrice = swaption.price(constantRates, volatility, timeStep, 4, false);

    // Calculate expected prices using the helper function
    double expectedPayerPrice = calculateExpectedSwaptionPrice(true, forwardSwapRate, strikeRate, volatility, 4, maturity, notional);
    double expectedReceiverPrice = calculateExpectedSwaptionPrice(false, forwardSwapRate, strikeRate, volatility, 4, maturity, notional);

    // Verify the prices
    REQUIRE(payerSwaptionPrice == Approx(expectedPayerPrice).epsilon(0.01));
    REQUIRE(receiverSwaptionPrice == Approx(expectedReceiverPrice).epsilon(0.01));
}

// Test with Different Volatilities
TEST_CASE("Swaption Pricing with Different Volatilities", "[Swaption]") {
    double strikeRate = 0.05;
    double maturity = 5.0;
    double notional = 1000.0;
    double swapLength = 1.0;
    double forwardSwapRate = 0.05;
    double timeStep = 0.01;

    // Calculate the necessary length for rates vector
    unsigned int requiredSteps = static_cast<unsigned int>((maturity + swapLength) / timeStep);

    // Generate constant rates
    std::vector<double> constantRates = generateConstantRates(forwardSwapRate, requiredSteps);

    // Create Swaption
    Swaption swaption(strikeRate, maturity, notional, swapLength);

    // Define different volatilities
    std::vector<double> volatilities = {0.1, 0.2, 0.3};

    for (double volatility : volatilities) {

        // Calculate Swaption price using the Swaption class
        double swaptionPrice = swaption.price(constantRates, volatility, timeStep, 4, true);

        // Calculate expected price using the helper function
        double expectedPrice = calculateExpectedSwaptionPrice(true, forwardSwapRate, strikeRate, volatility, 4, maturity, notional);

        // Verify the price
        REQUIRE(swaptionPrice == Approx(expectedPrice).epsilon(0.01));
    }
}