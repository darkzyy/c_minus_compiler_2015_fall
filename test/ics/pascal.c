int a[31][31];

/*
int ans[] = {1, 30, 435, 4060, 27405, 142506, 593775, 2035800, 5852925, 14307150, 30045015, 54627300, 86493225, 119759850, 145422675, 155117520, 145422675, 119759850, 86493225, 54627300, 30045015, 14307150, 5852925, 2035800, 593775, 142506, 27405, 4060, 435, 30, 1};
*/

int main() {
	int i, j;
	int N = 31;

	i = 0;
	while(i < N) {
		a[i][0] = a[i][i] = 1;
		i = i + 1;
	}

	i = 2;
	while(i < N) {
		j = 1;
		while(j < i) {
			a[i][j] = a[i - 1][j - 1] + a[i - 1][j];
			j = j + 1;
		}
		i = i + 1;
	}

	j = 0;
	while(j <= 30) {
		put_int(a[30][j]);
		j = j + 1;
	}

	return 0;
}