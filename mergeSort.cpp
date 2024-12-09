#include "mergeSort.h"
#include "merge.h"

void mergeSortReal(long long int A[], int p, int r) {
    if (r - p <= 1) return;
    int q = (r + p) / 2;

    mergeSortReal(A, p, q);
    mergeSortReal(A, q, r);

    merge(A, p, q, r);
}

void mergeSort(long long *A, int p, int r) {
    // dr b WHY on earth would we init this function with one-indexed boundaries???
    mergeSortReal(A, 0, r);
}
