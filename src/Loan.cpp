#include<iostream>
#include<stdexcept>
#include<sstream>
#include<string>
#include<cmath>
#include<iomanip>

#include "Loan.h"
#include "Utils.h" // *** ADDED: Assuming Utils.h contains the getOutputPrecision function ***

using namespace std;

LoanCalculator::LoanCalculator() :
 amountSet_(false),
 initialPayment_(0.0L),// Changing: Add L ( long double literal )
 interestSet_(false),
 paymentSet_(false),
 periodTotalSet_(false),
 periodElapsedSet_(false),
 openingFee_(0.0L),
 openingPercent_(0.0L)
{
}

//
// The actual calculation methods
//

/**
 * Loan balance after n payments have been made:
 *   B_n = A*(1+i)^n - (P/i)*((1+i)^n - 1)
 */
long double LoanCalculator::calculateLoanBalance()
{
 if(!amountSet_ || !interestSet_ || !periodElapsedSet_ || !paymentSet_)
 {
 throw invalid_argument("Must set loan amount, interest, and elapsed period for this calculation" );
 }

 // Handle 0% interest case to prevent division by zero
if (interestPeriodic_ == 0.0L) {
// Formula for 0%: Balance = Principal - Payment * Periods elapsed
return amount_ - (payment_ * periodElapsed_);
}

// Use powl() for high precision and long double types
return (amount_ * powl((1.0L + interestPeriodic_), periodElapsed_)) -
((payment_ / interestPeriodic_) * (powl((1.0L + interestPeriodic_), periodElapsed_)-1.0L));
}



/**
 * Payment amount on a loan:
 *   P = i*A / (1 - (1+i)^-N)
 */
long double LoanCalculator::calculatePayment()
{
 if(!amountSet_ || !interestSet_ || !periodTotalSet_)
 {
 throw invalid_argument("Must set loan amount, interest, and total period for this calculation" );
 }

 long double totalAmount = amount_ - initialPayment_;
 totalAmount = totalAmount + openingFee_ + (totalAmount * (openingPercent_/100.0L)); // Added L to 100.0

 if (interestPeriodic_ == 0.0L) {
 	if (periodTotal_ == 0) {
throw invalid_argument("Loan period cannot be zero." );
}
return totalAmount / periodTotal_;
}

// Use powl() for high precision
long double numerator = interestPeriodic_ * totalAmount;
long double denominator = 1.0L - powl((1.0L + interestPeriodic_), (-1 * periodTotal_));

return numerator / denominator;
}


/**
 * Number of payments on a loan:
 *   N = -log(1-i*A/P) / log(1+i)
 *      (You can use any logarithm base, as long as both logs use the same base.)
 *      Aunt Sally offers to lend you $3500 at 6% for that new home theater system you want.
 *      If you pay her back $100 a month, how long will it take?
 *      Solution:  6% per year is 0.5% per month, or 0.005. P = 100 and A = 3500. N = 38.57
 */
long double LoanCalculator::calculateNumberPayments()
{
 if(!amountSet_ || !interestSet_ || !paymentSet_)
 {
 throw invalid_argument("Must set loan amount, interest, and payment for this calculation" );
 }
 // Use long double for local calculation variables
long double amount = amount_ - initialPayment_;

 if (interestPeriodic_ == 0.0L) {
// Formula for 0%: N = Amount / Payment
return amount / payment_;
}

 long double ratio = interestPeriodic_ * amount / payment_;

 // Check for payment too low to pay off the loan
if (ratio >= 1.0L) {
throw invalid_argument("Payment is too low to cover interest/principal and pay off the loan." );
}

 return (-1.0L * log10l(1.0L - ratio)) / log10l(1.0L + interestPeriodic_);

}

/**
 * Original loan amount:
 *   A = (P/i)*(1 - (1+i)^-N)
 */
long double LoanCalculator::calculateLoanAmount()
{
if(!paymentSet_ || !interestSet_ || !periodTotalSet_)
{
throw invalid_argument("Must set payment, interest, and total period for this calculation" );
}

// Handle 0% interest case
if (interestPeriodic_ == 0.0L) {
// Formula for 0%: A = Payment * Total Periods
return payment_ * periodTotal_;
}

// Use powl() for high precision
return (payment_ / interestPeriodic_) *
(1.0L - powl((1.0L + interestPeriodic_), (-1 * periodTotal_)));
}

/**
 * Interest Rate:
 *   i = (((1 + P/A)^(1/q) - 1 )^q - 1)  NOTICE: This is an approximate not an exact solution
 *   where q = log(1+1/N) / log(2)
*/
long double LoanCalculator::calculateInterestRate()
{
if(!amountSet_ || !paymentSet_ || !periodTotalSet_)
{
throw invalid_argument("Must set amount, payment, and total period for this calculation" );
}

// Use long double for local variables
long double actualAmount = amount_ - initialPayment_;

if (actualAmount <= 0.0L) {
throw invalid_argument("Actual loan amount must be positive for interest calculation." );
}
// Use log10l() and powl() for high precision
long double q = log10l(1.0L + 1.0L/periodTotal_) / log10l(2.0L);
long double monthlyInterest = powl((powl((1.0L + payment_/actualAmount), 1.0L/q) - 1.0L), q) - 1.0L;

return monthlyInterest * 12.0L * 100.0L; // Convert to yearly percentage
}

long double LoanCalculator::calculateEffectiveInterestRate()
{
if(!amountSet_ || !periodTotalSet_)
{
throw invalid_argument("Must set amount and total period for this calculation" );
}

// Use long double for local variables
long double payment = calculatePayment();
long double totalAmountFinanced = amount_ - initialPayment_ + openingFee_ + (amount_ - initialPayment_) * (openingPercent_/100.0L);

if (totalAmountFinanced <= 0.0L) {
throw invalid_argument("Total amount financed must be positive for effective interest calculation." );
}

// Use log10l() and powl() for high precision
long double q = log10l(1.0L + 1.0L/periodTotal_) / log10l(2.0L);
long double monthlyInterest = powl((powl((1.0L + payment/totalAmountFinanced), 1.0L/q) - 1.0L), q) - 1.0L;

return monthlyInterest * 12.0L * 100.0L; // Convert to yearly percentage
}

std::string LoanCalculator::toString()
{
 std::stringstream ss;

 // Set precision for consistent output (Bug Fix c: Load from config.txt)
 // NOTE: You must implement Utils::getOutputPrecision() to read the value from config.txt.
 int precision = getOutputPrecision(); 
 ss << std::fixed << std::setprecision(precision);


 //ss << "LoanCalculator set values:\n";

 if(amountSet_)
 {
 ss << "Initial Amount:      " << amount_ << "\n";
 }

 if(initialPayment_ != 0.0L)
 {
 ss << "Initial Payment:     " << initialPayment_ << "\n";
 ss << "Actual Loan Amount:  " << (amount_ - initialPayment_) << "\n";
 }

 if(interestSet_)
 {
 ss << "Yearly Interest:     " << interest_ << "%\n";
 //ss << "Yearly Interest:     " << interest_
 //   << "\nMonthly Interest:    " << interestPeriodic_ << "\n";
 }

 if(paymentSet_)
 {
 ss << "Monthly payment:     " << payment_ << "\n";
 }

 if(periodTotalSet_)
 {
 ss << "Loan Period:         " << periodTotal_ << " months\n";
 }

 if(periodElapsedSet_)
 {
  ss << "Elapsed Period:      " << periodElapsed_ << " months\n";
 }

 if(openingFee_ != 0.0L)
 {
 ss << "Opening Fee:       " << openingFee_ << "\n";
 }

 if(openingPercent_ != 0.0L)
 {
 ss << "Opening Fee %:       " << openingPercent_ << "% = "
<< openingPercent_/100.0L*(amount_ - initialPayment_) << "\n";
 }

 return ss.str();
}
