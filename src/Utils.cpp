#include "Utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath> // Include for long double math consistency

/**
 * @brief Reads the output precision value from 'config.txt'.
 * * This function searches for a line "OUTPUT_PRECISION=X" in the config.txt file.
 * If the file is not found or the value is invalid, it defaults to 2.
 * This fixes the hardcoded value bug (Assignment Task 2.c).
 * * @return int The output precision value (default 2).
 */
int getOutputPrecision() {
    std::ifstream configFile("config.txt");
    std::string line;
    const std::string key = "OUTPUT_PRECISION=";
    int precision = 2; // Default value if reading fails

    if (configFile.is_open()) {
        while (std::getline(configFile, line)) {
            // Check if line starts with the key
            if (line.rfind(key, 0) == 0) {
                std::string valueStr = line.substr(key.length());
                try {
                    // Safely convert string to integer
                    precision = std::stoi(valueStr);
                    // Basic sanity check for precision
                    if (precision < 0 || precision > 15) {
                        precision = 2;
                    }
                    configFile.close();
                    return precision;
                } catch (const std::exception& e) {
                    std::cerr << "Warning: Invalid precision value in config.txt. Using default (2).\n";
                    configFile.close();
                    return 2;
                }
            }
        }
        configFile.close();
    } else {
        // This warning is helpful during debugging
        std::cerr << "Warning: config.txt not found. Using default output precision (2).\n";
    }

    return precision;
}

/**
 * @brief Validates core loan parameters (principal, rate, tenure).
 * * @param principal The loan principal amount.
 * @param annual_rate The yearly interest rate (percent).
 * @param tenure_months The total loan period in months.
 * @return bool True if all parameters are valid, false otherwise.
 * * Implements Assignment Bug Fix 2.a: Input validation.
 */
bool validateLoanParameters(long double principal, long double annual_rate, int tenure_months) {
    bool isValid = true;

    if (principal <= 0.0L) {
        std::cerr << "ERROR: Loan principal amount must be positive." << std::endl;
        isValid = false;
    }

    // Annual rate can be 0% but not negative
    if (annual_rate < 0.0L) {
        std::cerr << "ERROR: Annual interest rate cannot be negative." << std::endl;
        isValid = false;
    }

    if (tenure_months <= 0) {
        std::cerr << "ERROR: Loan tenure must be a positive number of months." << std::endl;
        isValid = false;
    }

    return isValid;
}
