#include <stdio.h>

int main() {
    float income, tax = 0;
    
    printf("\nINCOME TAX CALCULATOR 2024-25\n");
    printf("==============================\n");
    
    printf("Enter annual income (₹): ");
    scanf("%f", &income);
    
    if (income <= 250000) {
        tax = 0;
    }
    else if (income <= 500000) {
        tax = (income - 250000) * 0.05;
    }
    else if (income <= 1000000) {
        tax = 12500 + (income - 500000) * 0.20;
    }
    else {
        tax = 112500 + (income - 1000000) * 0.30;
    }
    
    // Add cess (4%)
    float cess = tax * 0.04;
    tax += cess;
    
    printf("\n=== TAX CALCULATION ===\n");
    printf("Annual Income: ₹%.2f\n", income);
    printf("Basic Tax: ₹%.2f\n", tax - cess);
    printf("Cess (4%%): ₹%.2f\n", cess);
    printf("Total Tax: ₹%.2f\n", tax);
    printf("Effective Rate: %.2f%%\n", (tax/income)*100);
    
    return 0;
}