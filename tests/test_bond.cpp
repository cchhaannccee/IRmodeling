#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <cmath>
#include "Bond.hpp"

// Generate a vector of constant interest rates
std::vector<double> constRates(double rate, unsigned int steps) {
    std::vector<double> rates(steps);
    for (unsigned int i = 0; i < steps; ++i) {
        rates[i] = rate;
    }
    return rates;
}

// Helper function to calculate expected bond price using constant rates
double calculateExpectedBondPrice(double faceValue, double maturity, double couponRate, double frequency, double intRate) {
    double presentValue = 0.0;
    double cashFlow = faceValue * couponRate * frequency;
    int couponPeriods = static_cast<int>(maturity / frequency);

    for (int i = 0; i < couponPeriods; ++i) {
        double time = (i + 1) * frequency;
        presentValue += cashFlow * std::exp(-intRate * time);
    }

    presentValue += faceValue * std::exp(-intRate * maturity);
    return presentValue;
}

//tests compare constant rates results to bond pricing formula
TEST_CASE("constant rates", "[tests]")
{
    //bond parameters to test
    double c_face[] = {.1, 1, 100, 5787, 1000};
    double c_maturity[] = {.1, .7, 1.0, 5.55, 10};
    double c_coupon_rate[] = {.001, .01, .05, 1, 5};
    double c_frequency[] = {.001, .1, .25, 1, 2};
    double c_int_rate[] = {.02, .05, .07};
    double c_time_step[] = {.01, .1};

    for (double face : c_face) {
        for (double maturity : c_maturity) {
            for (double coupon_rate : c_coupon_rate) {
                for (double frequency : c_frequency) {
                    for (double int_rate : c_int_rate) {
                        for (double time_step : c_time_step) {
                            Bond bond(face, maturity, coupon_rate, frequency);
                            std::vector<double> constant_rates = constRates(int_rate, static_cast<int>(maturity / time_step) + 1);
                            double bond_price = bond.price(constant_rates, time_step);
                            double expected_price = calculateExpectedBondPrice(face, maturity, coupon_rate, frequency, int_rate);

                            REQUIRE(bond_price == Approx(expected_price).epsilon(0.01));
                        }
                    }
                }
            }
        }
    }
}

TEST_CASE("face value scaling", "[Bond]") {
    std::vector<double> constant_rates = constRates(0.05, 500);
    Bond bond_one(1, 2, 0.05, 0.5);
    Bond bond_thousand(1000, 2, 0.05, 0.5);
    double bond_one_price = bond_one.price(constant_rates, 0.05);
    double bond_thousand_price = bond_thousand.price(constant_rates, 0.05);

    REQUIRE(bond_one_price * 1000 == Approx(bond_thousand_price).epsilon(0.01));
}

TEST_CASE("bond rate equals interest rate", "[Bond]") {
    std::vector<double> constant_rates = constRates(0.05, 500);
    Bond bond(1000, 2, 0.05, 0.5);
    double bond_price = bond.price(constant_rates, 0.05);
    REQUIRE(bond_price == Approx(1000).epsilon(0.01));
}