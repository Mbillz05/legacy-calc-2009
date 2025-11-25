#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <string>
#include <stdexcept>

int getOutputPrecision();


// Function to validate the principal, interest rate, and tenure
bool validateLoanParameters(long double principal, long double annual_rate, int tenure_months);

#endif // UTILS_H
