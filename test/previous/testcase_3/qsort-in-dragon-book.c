int qsort(int A[100], int m, int n)
{
    int i, j;
    int v, x;
    if (n <= m) return 0;

    i = m-1; j = n; v = A[n];
    while (1) {
        i = i + 1; while (A[i] < v) i = i + 1;
        j = j - 1; while (A[j] > v) j = j - 1;

        if (i >= j) {
            x = A[i]; A[i] = A[n]; A[n] = x;
            qsort(A, m, j); qsort(A, i + 1, n);
            return 0;
        }

        x = A[i]; A[i] = A[j]; A[j] = x;
    }

    return 0;
}

int main()
{
    int a[100];
    int idx = 0;

    while (idx < 100) {
        a[idx] = idx;
        idx = idx + 1;
    }

    qsort(a, 0, 99);
    idx = 0;
    while (idx < 100) {
        write(a[idx]);
        idx = idx + 1;
    }
    return 0;
}
