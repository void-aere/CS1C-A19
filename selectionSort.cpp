#include <iostream>

void selectionSort(long long int A[], int n) {
	int mindex;
	long long temp;
	for (int i = 0; i < n - 1; ++i) {
		mindex = i;
		for (int j = i + 1; j < n; ++j) {
			if (A[j] < A[mindex]) {
				mindex = j;
			}
		}
		if (mindex != i) {
			temp = A[i];
			A[i] = A[mindex];
			A[mindex] = temp;
		}
	}
}
