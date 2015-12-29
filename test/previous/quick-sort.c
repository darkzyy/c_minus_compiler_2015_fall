struct arr {
    int r[100];
};
int partition(struct arr x,int p, int q) {
	int pivot = x.r[p];
	int ii = p, j = q;
	while(ii < j) {
		while(ii < j && x.r[j] > pivot) j = j - 1;
		x.r[ii] = x.r[j];

		while(ii < j && x.r[ii] <= pivot) ii = ii + 1;
		x.r[j] = x.r[ii];
	}

	x.r[ii] = pivot;
	return ii;
}

int quick_sort(struct arr x2,int p2, int q2) {
	int m;
	if(p2 >= q2) return 0;

	m = partition(x2,p2, q2);
	quick_sort(x2,p2, m - 1);
	quick_sort(x2,m + 1, q2);
    return 0;
}

int main() {
    struct arr rr;
	int N = 100;
	int i = 0;
	while(i < N) {
		rr.r[i] = N-i;
		i = i + 1;
	}

	quick_sort(rr, 0, N - 1);

	i = 0;
	while(i < N) {
        write(rr.r[i]);
		i = i + 1;
	}

	return 0;
}
