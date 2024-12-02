#ifndef INSERTIONSORT_Included
#define INSERTIONSORT_Included

/**
  * You must fashion your code from the pseudo code provided on pg. 19
  * of your text book.
  *
  * Insertion sort algorithm involves the sorted list created based on an
  * iterative comparison of each element in the list with its adjacent element.
  *
  * An index pointing at the current element indicates the position of the sort.
  *
  * At the beginning of the sort (index=0), the current value is compared to the
  * adjacent value to the left. If the value is greater than the current value,
  * no modifications are made to the list; this is also the case if the adjacent
  * value and the current value are the same numbers. 
  *
  * However, if the adjacent value to the left of the current value is lesser,
  * then the adjacent value position is moved to the left, and only stops moving
  * to the left if the value to the left of it is lesser.

 * @param A[]  The array to be sorted.
 * @param n The number of elements in A.
 */
void insertionSort(long long int A[], int n);


#endif
