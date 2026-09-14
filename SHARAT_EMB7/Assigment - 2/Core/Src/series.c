/*
 * series.c
 *
 *  Created on: Aug 26, 2026
 *      Author: SHARAT
 */
#include"series.h"
/**
 * @brief prime number series print function
 * 
 * @param n 
 * @param primearray 
 * @param retcount 
 */
void PrimeNumber(int n,int primearray[],int *retcount)
 {
    // Array to store the prime numbers

    // Keeps track of how many prime numbers are found
    int count = 0;

    // Check every number from 2 up to n
    for (int number = 2; number <= n; number++)
    {
        // Assume the number is prime
        int isPrime = 1;

        // Check whether number is divisible by any value
        // from 2 up to the square root of number
        for (int i = 2; i * i <= number; i++)
        {
            // If number is exactly divisible by i,
            // then it is not a prime number
            if (number % i == 0)
            {
                isPrime = 0;
                break;
            }
        }

        // If isPrime is still 1, the number is prime
        if (isPrime)
        {
            // Store the prime number in the array
            primearray[count++] = number;
        }
    }
    *retcount=count;//Returning the count to the pointer

}
