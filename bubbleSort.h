#ifndef BUBBLESORT_Included
#define BUBBLESORT_Included

//#include <stddef.h> // For size_t

/**
 * You must fashion your code after the pseudo code on pg. 
 * 46 of your textbook. You must deal with any "off-by-one"
 * errors that you may experience accordingly.
 *
 * The bubble sort makes multiple passes through a list.
 * It compares adjacent items and exchanges those that are 
 * out of order. Each pass through the list places the next
 * largest value in its proper place. In essence, each item
 * “bubbles” up to the location where it belongs.
 *
 * @param A[]  The array to be sorted.
 * @param n The number of elements in A.
 */
void bubbleSort(long long int A[], int n);


#endif
