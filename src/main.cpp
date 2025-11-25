#include "Loan.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>

// Function to print usage guide
void printUsage(const std::string& name) {
    std::cerr << "Usage: " << name << " -a <amount> -i <interest> -n <months>" << std::endl;
    std::cerr << "  -a: Loan amount (e.g., 100000.0)" << std::endl;
    std::cerr << "  -i: Annual interest rate in percent (e.g., 6.75)" << std::endl;
    std::cerr << "  -n: Total number of months (e.g., 360)" << std::endl;
    std::cerr << "Example: " << name << " -a 100000 -i 12 -n 360" << std::endl;
}

int main(int argc, char* argv[]) {
    LoanCalculator calc;
    bool allParamsSet = false;

    // --- Command Line Argument Parsing ---
    long double amount = 0.0L;
    long double interest = 0.0L;
    int period = 0;

    if (argc != 7) {
        printUsage(argv[0]);
        return 1;
    }

    try {
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "-a" && i + 1 < argc) {
                amount = std::stold(argv[++i]);
            } else if (arg == "-i" && i + 1 < argc) {
                interest = std::stold(argv[++i]);
            } else if (arg == "-n" && i + 1 < argc) {
                period = std::stoi(argv[++i]);
            } else if (arg == "-h" || arg == "--help") {
                printUsage(argv[0]);
                return 0;
            } else {
                std::cerr << "Error: Unknown or incomplete argument '" << arg << "'" << std::endl;
                printUsage(argv[0]);
                return 1;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: Invalid numeric value provided. " << e.what() << std::endl;
        printUsage(argv[0]);
        return 1;
    }

    // --- Input Validation (Bug Fix 2.a) ---
    if (!validateLoanParameters(amount, interest, period)) {
        return 1;
    }

    // --- Core Logic ---
    try {
        calc.setAmount(amount);
        calc.setInterest(interest);
        calc.setPeriodTotal(period);

        // Get precision from config file (Bug Fix 2.c)
        int precision = getOutputPrecision();

        long double emi = calc.calculatePayment();

        // Output result
        std::cout << "\n--- Loan Calculation Summary ---" << std::endl;
        std::cout << std::fixed << std::setprecision(precision);
        std::cout << "Loan Amount:      $" << calc.getAmount() << std::endl;
        std::cout << "Annual Rate:      " << calc.getInterest() << "%" << std::endl;
        std::cout << "Total Periods:    " << calc.getPeriodTotal() << " months" << std::endl;
        std::cout << "Monthly Payment:  $" << emi << std::endl;
        std::cout << "--------------------------------" << std::endl;
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Calculation Error: " << e.what() << std::endl;
        return 1;
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Fatal Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
