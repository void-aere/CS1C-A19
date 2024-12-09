#include <algorithm>

void merge(long long int A[], int p, int q, int r) {
    // Copy the first subarray into its own memory before merging
    long long temp[q - p];
    std::copy(A + p, A + q, temp);

    int ip = p;
    int iq = q;
    for (int i = p; i < r; ++i) {
        if (ip >= q) {
            // The first subarray is exhausted, and the other is already in place, so we're done
            // A[i] = A[iq];
            // ++iq;
            break;
        } else if (iq >= r) {
            // The second subarray is exhausted, so pull from the first
            A[i] = temp[ip - p];
            ++ip;
        } else {
            // Pull from whichever subarray has the lowest next value
            if (temp[ip - p] < A[iq]) {
                A[i] = temp[ip - p];
                ++ip;
            } else {
                A[i] = A[iq];
                ++iq;
            }
        }
    }
}
