
#include <iostream>
#include <vector>
#include <fstream>
#include "VasicekModel.hpp"
#include "CIRModel.hpp"
#include "Bond.hpp"
#include "RateSimulator.hpp"
#include "Swaption.hpp"

// Function to save simulation results to a CSV file
void saveSimRes(const std::vector<double>& VasicekRates, const std::vector<double> CIRRates,
				const std::string& filename){
	// Open the output file and check opening is successful
	std::ofstream outputFile(filename);
	if (!outputFile.is_open()){
		std::cerr << "Failed to open file: " << filename << std::endl;
		return;
	}

	// Write header
	outputFile << "Step, Vasicek Rate, CIR Rate\n";

	// Write data
	for (size_t i = 0; i < VasicekRates.size(); ++i){
		outputFile << i << "," << VasicekRates[i] << "," << CIRRates[i] << "\n";
	}

	// Debug output to verify data writing
    std::cout << "Wrote " << VasicekRates.size() << " lines to " << filename << std::endl;

    // Close the file
    outputFile.close();

	// Check output was written successfully
    if (outputFile.fail()) {
        std::cerr << "Failed to properly write the file: " << filename << std::endl;
    } else {
        std::cout << "Successfully wrote the file: " << filename << std::endl;
    }
	
}

int main(){

	// Create models, bond, swaption and rate simulator
	VasicekModel vasicek(0.1,0.05,0.01);
	CIRModel cir(0.1,0.05,0.01);
	Bond bond(1000, 10, 0.05, 0.5);
	Swaption swaption(0.05, 10, 1000, 1);
	RateSimulator simulator;

	// Define simulation parameters
	double initialRate = 0.03;
	double timeStep = 0.05;
	int steps = 20 / timeStep;

	// Simulate interest rate paths using Vasicek and CIR models
	std::vector<double> VasicekRates = simulator.simulatePaths(vasicek, initialRate, timeStep, steps);
	std::vector<double> CIRRates = simulator.simulatePaths(cir, initialRate, timeStep, steps);

	// Price the bond using the simulated rates
	double VasicekBondPrice = bond.price(VasicekRates, timeStep);
	double CIRBondPrice = bond.price(CIRRates, timeStep);

	// Price the swaption using the simulated rate paths
    double VasicekSwaptionPricePayer = swaption.price(VasicekRates, 0.2, timeStep, 4, true); 
    double VasicekSwaptionPriceReceiver = swaption.price(VasicekRates, 0.2, timeStep, 4, false); 
    double CIRSwaptionPricePayer = swaption.price(CIRRates, 0.2, timeStep, 4, true); 
    double CIRSwaptionPriceReceiver = swaption.price(CIRRates, 0.2, timeStep, 4, false); 

	// Display bond prices
	std::cout << "Vasicek Model Bond Price: " << VasicekBondPrice << std::endl;
	std::cout << "CIR Model Bond Price: " << CIRBondPrice << std::endl;
	std::cout << "Vasicek Model Payer Swaption Price: " << VasicekSwaptionPricePayer << std::endl;
    std::cout << "Vasicek Model Receiver Swaption Price: " << VasicekSwaptionPriceReceiver << std::endl;
    std::cout << "CIR Model Payer Swaption Price: " << CIRSwaptionPricePayer << std::endl;
    std::cout << "CIR Model Receiver Swaption Price: " << CIRSwaptionPriceReceiver << std::endl;


	// Save the simulation results to a CSV
	saveSimRes(VasicekRates, CIRRates, "data/output.csv");
}

