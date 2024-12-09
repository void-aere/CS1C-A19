void insertionSort(long long int A[], int n) {
    long long temp;
    int j;
    for (int i = 1; i < n; ++i) {
        if (A[i] >= A[i - 1]) continue;
        temp = A[i];
        j = i - 1;
        while (j >= 0 && A[j] > temp) {
            A[j + 1] = A[j];
            --j;
        }
        A[j + 1] = temp;
    }
}
