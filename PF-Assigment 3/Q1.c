#include <stdio.h>
#include <stdlib.h>

double repay(double loan, double rate, double payment, int year) {
    if (loan <= 0.0) return 0.0;
    if (year > 1000) {
        printf("Exceeded 1000 years — aborting to avoid infinite recursion.\n");
        exit(1);
    }
    double interest = loan * rate;
    if (payment <= interest + 1e-12) {
        printf("Year %d: Payment %.2f is not enough to cover interest %.2f — loan will never be repaid.\n", year, payment, interest);
        exit(1);
    }
    double totalDue = loan + interest;
    double paidThisYear = payment >= totalDue ? totalDue : payment;
    double remaining = totalDue - paidThisYear;
    printf("Year %d: Interest = %.2f, Paid = %.2f, Remaining = %.2f\n", year, interest, paidThisYear, remaining);
    if (remaining <= 1e-9) return paidThisYear;
    return paidThisYear + repay(remaining, rate, payment, year + 1);
}

int main(void) {
    double principal, ratePercent, payment;
    if (scanf("%lf %lf %lf", &principal, &ratePercent, &payment) != 3) {
        printf("Input error. Provide: principal rate_percent annual_payment\n");
        return 1;
    }
    if (principal <= 0.0 || payment <= 0.0) {
        printf("Principal and payment must be positive.\n");
        return 1;
    }
    double rate = ratePercent / 100.0;
    printf("Starting loan = %.2f, rate = %.3f (decimal), annual payment = %.3f\n", principal, rate, payment);
    double total = repay(principal, rate, payment, 1);
    printf("Total paid = %.2f\n", total);
    return 0;
}

