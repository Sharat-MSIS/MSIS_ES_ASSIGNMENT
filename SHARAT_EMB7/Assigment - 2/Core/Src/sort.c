/*
 * sort.c
 *
 *  Created on: Aug 26, 2026
 *      Author: SHARAT
 */
#include"sort.h"
/**
 * @brief insertion sort technique
 * 
 * @param arr 
 * @param n 
 */
void insertionsort(int arr[],int n)
{
   
    for (int i = 0; i < n - 1; i++)
{
    // Assume the current position contains
    // the minimum element
    int min = i;
    // Search for the smallest element
    // in the remaining unsorted array
    for (int j = i + 1; j < n; j++)
    {
        // If a smaller element is found,
        // update the minimum element index
        if (arr[j] < arr[min])
        {
            min = j;
        }
    }
    // Swap the minimum element with
    // the element at the current position
    int temp = arr[i];
    arr[i] = arr[min];
    arr[min] = temp;
}


}
