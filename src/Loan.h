#ifndef LOAN_H_INCLUDED
#define LOAN_H_INCLUDED

/*
Formulas from: http://oakroadsystems.com/math/loan.htm
P = i*A / (1 - (1+i)^-N)
A = P * (1 - (1+i)^-N) / i
N = -log(1 - i*A/P) / log(1+i)
B = A * (1+i)^n - P * ((1+i)^n - 1) / i
Where:
A is the loan amount (principal)
i is the periodic (monthly) interest rate
N is the total number of periods (months)
P is the payment (EMI)
B is the balance after n periods
n is the elapsed number of periods
*/

#include <string>
#include <cmath> // Include for powl() usage

class LoanCalculator
{
public:
    LoanCalculator();
    ~LoanCalculator() {}

    //
    // Setters and Getters
    //

    /**
     * Total loan amount A
     */
    inline void setAmount(long double A) { amount_ = A; amountSet_ = true; }
    inline long double getAmount() const { return amount_; }

    /**
     * Initial down payment
     */
    inline void setInitialPayment(long double initialA) { initialPayment_ = initialA; }
    inline long double getInitialPayment() const { return initialPayment_; }

    /**
     * Yearly interest rate i as in 6.75
     * Internally .0675/12 will be used
     * If 6.75 is passed to setInterest()
     * getInterest() will return 6.75
     * getPeriodicInterest() will return .0675/12.0
     */
    void setInterest(long double i) { interest_ = i; interestPeriodic_ = i/100.0L/12.0L; interestSet_ = true; }
    inline long double getInterest() const { return interest_; }
    inline long double getPeriodicInterest() const { return interestPeriodic_; }

    void setPayment(long double P) { payment_ = P; paymentSet_ = true; }
    inline long double getPayment() const { return payment_; } // Returns long double

    void setPeriodTotal(int N) { periodTotal_ = N; periodTotalSet_ = true; }
    inline int getPeriodTotal() const { return periodTotal_; }

    void setPeriodElapsed(int n) { periodElapsed_ = n; periodElapsedSet_ = true; }
    inline int getPeriodElapsed() const { return periodElapsed_; }

    inline void setOpeningFee(long double fee) { openingFee_ = fee; }
    inline long double getOpeningFee() const { return openingFee_; }

    inline void setOpeningPercent(long double percent) { openingPercent_ = percent; }
    inline long double getOpeningPercent() const { return openingPercent_; }

    inline void reset() {
        amount_ = initialPayment_ = interest_ = interestPeriodic_ = payment_ = openingFee_ = openingPercent_ = 0.0L;
        periodTotal_ = periodElapsed_ = 0;
        amountSet_ = interestSet_ = paymentSet_ = periodTotalSet_ = periodElapsedSet_ = false;
    }

    //
    // The actual calculation methods
    //

    long double calculateLoanBalance();
    long double calculatePayment();
    long double calculateNumberPayments();
    long double calculateLoanAmount();
    long double calculateInterestRate();
    // The effective interest rate, once fees have been applied
    long double calculateEffectiveInterestRate();

    std::string toString();

private:
    long double amount_;         // loan amount
    bool amountSet_;

    long double initialPayment_;   // initial down payment

    long double interest_;         // interest rate, something like 6.75
    long double interestPeriodic_; // this will be .0675/12
    bool interestSet_;

    long double payment_;        // payment amount
    bool paymentSet_;

    int periodTotal_;    // total payment periods
    bool periodTotalSet_;

    int periodElapsed_;  // number of elapsed payment periods
    bool periodElapsedSet_;

    // These two are used if loans charge a fee opening fee or percentage
    long double openingFee_;
    long double openingPercent_;
};

#endif // LOAN_H_INCLUDED
