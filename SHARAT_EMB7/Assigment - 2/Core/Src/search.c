/*
 * search.c
 *
 *  Created on: Aug 26, 2026
 *      Author: SHARAT
 */
#include"search.h"
#include<stdio.h>
/**
 * @brief 
 * tirnary function to find the key in a given array
 * @param arr 
 * @param n 
 * @param key 
 * @return int 
 */
int ternary(int arr[], int n, int key)
{
    int high = n - 1;   // Index of the last element
    int low = 0;        // Index of the first element

    while (low <= high)
    {
        // Divide the current search range into 3 parts
        int mid1 = low + ((high - low) / 3);       // First middle position
        int mid2 = high - ((high - low) / 3);      // Second middle position

        // Check if key is present at mid1
        if (arr[mid1] == key)
        {
            return mid1;    // Key found at mid1
        }

        // Check if key is present at mid2
        if (arr[mid2] == key)
        {
            return mid2;    // Key found at mid2
        }

        // If key is smaller than arr[mid1],
        // search in the first third
        if (arr[mid1] > key)
        {
            high = mid1 - 1;
        }

        // If key is greater than arr[mid2],
        // search in the third third
        else if (arr[mid2] < key)
        {
            low = mid2 + 1;
        }

        // Otherwise, key lies between mid1 and mid2
        else
        {
            low = mid1 + 1;
            high = mid2 - 1;
        }
    }

    // Key was not found
    return -1;
}
