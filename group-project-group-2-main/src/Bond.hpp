#pragma once

#include <vector>

// Class for bonds
class Bond{
    private:
        double FaceValue;
        double Maturity;
        double couponRate;
        double Frequency;

    public:

        // Constructor for Bond class        
        Bond(double FV, double Mat, double CR, double f);

        // Calculate bond price
        double price(const std::vector<double>& rates, double timeStep) const;
};

