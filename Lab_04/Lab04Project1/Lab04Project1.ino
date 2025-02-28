#include <stdio.h>

#define BITSIZE 14  // 14-bit SAR ADC resolution

float Vref = 3.0;  // Reference voltage
float Vin = 0.42;  // Change this for different test values
float quantized = 0;
int bits[BITSIZE];

void main(void) {
    float thresh = Vref / 2;  // Start with half of Vref
    float step = Vref / 2;    // Step size starts at Vref/2
    
    for (int count = 0; count < BITSIZE; count++) {
        step /= 2;  // Halve the step size at each iteration
        
        if (Vin >= thresh) {
            bits[count] = 1;
            quantized += step;  // update the quantized value
            thresh += step;  // move threshold up
        } else {
            bits[count] = 0;
            // Don't move `thresh` down; it remains the same
        }
    }

    // Print results
    printf("Digital Output: ");
    for (int i = 0; i < BITSIZE; i++) {
        printf("%d", bits[i]);
    }
    printf("\nQuantized Value: %.4f V\n", quantized);
}
