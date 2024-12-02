#ifndef MERGE_Included
#define MERGE_Included

/**
 * You must fashion your code after the pseudo code on pg.
 * 36 of your text book.
 *
 * The merge function copies the two subarrays
 * A[p:q] and A[q + 1:r] into temporary arrays L and R
 * "Left" and "Right", and then it merges the values in
 * L and R back into A[p:r].
 *
 * @param A[]  The array to be sorted.
 * @param p, q, and r are indices into the array such
 *        that p <= q < r.
 *
 * The procedure assumes that the adjacent subarrays
 * A[p:q] and A[q + 1:r] were already recursively sorted.
 *
 * This function merges the two sorted subarrays to form
 * a single sorted subarray that replaces the current
 * subarray A[p:r].
 */
void merge(long long int A[], int p, int q, int r);


#endif
