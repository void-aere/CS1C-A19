#ifndef MERGESORT_Included
#define MERGESORT_Included

/**
 * Ensure that you fashion your code from the pseudo code on
 * pg. 39 of your text book.
 *
 * mergeSort sorts the elements in the subarray A[p:r]. If
 * p equals r, the subarray has just 1 element and is therefore
 * already sorted. Otherwise, we must have p < r, and mergeSort
 * runs the divide, conquer, and combine steps.
 *
 * @param A[]  The array to be sorted.
 * @param p and r are the indices of the array that is to be sorted
 */
void mergeSort(long long int A[], int p, int r);


#endif
