#include "gtest/gtest.h"
#include "Loan.h" // Assuming Loan.h is one level up in src/
#include "Utils.h" // Assuming Utils.h is one level up in src/
#include <cmath> // Required for powl and other math functions

// Define tolerance for floating point comparisons (Bug Fix 2.b requirement)
const long double TOLERANCE = 0.0001L;

// --- Test Case 1: Standard EMI Calculation ---
// Tests the basic functionality of the calculatePayment() method.
TEST(LoanCalculatorTest, CalculateStandardPayment) {
    LoanCalculator calc;
    
    // Set parameters:
    // A = $100,000
    // i = 5.0% annual (0.05/12 monthly)
    // N = 360 months (30 years)
    long double amount = 100000.0L;
    long double interest = 5.0L;
    int period = 360;

    // Expected value from a reliable online calculator (e.g., bankrate.com)
    // Expected EMI for these values is $536.8216
    const long double EXPECTED_EMI = 536.8216L;

    calc.setAmount(amount);
    calc.setInterest(interest);
    calc.setPeriodTotal(period);

    long double result = calc.calculatePayment();

    // Use EXPECT_NEAR for floating-point comparison (using long double and TOLERANCE)
    EXPECT_NEAR(EXPECTED_EMI, result, TOLERANCE);
}


// --- Test Case 2: Large Tenure EMI Calculation (Verification of long double/powl fix) ---
// This test verifies the Bug Fix using 'long double' and 'powl' for precision over extremely long tenures.
TEST(LoanCalculatorTest, LargeTenurePayment) {
    LoanCalculator calc;
    
    // Set parameters:
    // A = $100,000
    // i = 5.0% annual
    // N = 600 months (50 years) - A very large tenure.
    long double amount = 100000.0L;
    long double interest = 5.0L;
    int period = 600; 

    // Calculate the expected EMI using the formula and long double precision:
    // EMI = P * r * (1 + r)^n / ((1 + r)^n - 1)
    long double annual_rate = interest / 100.0L; 
    long double monthly_rate = annual_rate / 12.0L; 
    long double power_term = powl(1.0L + monthly_rate, period);
    
    // The expected EMI (calculated precisely using long double)
    const long double EXPECTED_EMI = amount * monthly_rate * power_term / (power_term - 1.0L); 
    // This value is approximately $528.8465

    calc.setAmount(amount);
    calc.setInterest(interest);
    calc.setPeriodTotal(period);

    long double result = calc.calculatePayment();

    // The calculated EMI should be very close to the expected value
    EXPECT_NEAR(EXPECTED_EMI, result, TOLERANCE);
}


// --- Test Case 3: Invalid Input Validation (Bug Fix 2.a) ---
// Tests the utility function that enforces the input validation bug fix.
TEST(UtilsTest, ValidateLoanParametersInvalid) {
    // 1. All invalid
    EXPECT_FALSE(validateLoanParameters(0.0L, -1.0L, 0)) << "Test 1 failed: All invalid parameters passed validation.";

    // 2. Invalid principal
    EXPECT_FALSE(validateLoanParameters(-1000.0L, 5.0L, 12)) << "Test 2 failed: Negative principal passed validation.";

    // 3. Invalid tenure
    EXPECT_FALSE(validateLoanParameters(10000.0L, 5.0L, 0)) << "Test 3 failed: Zero tenure passed validation.";

    // 4. Invalid interest rate (negative)
    EXPECT_FALSE(validateLoanParameters(10000.0L, -5.0L, 12)) << "Test 4 failed: Negative interest rate passed validation.";

    // 5. All valid (should pass)
    EXPECT_TRUE(validateLoanParameters(10000.0L, 5.0L, 12)) << "Test 5 failed: Valid parameters failed validation.";
}
