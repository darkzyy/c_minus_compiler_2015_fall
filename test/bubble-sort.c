int main() {
    int a[100];
    int i = 0,j=0;
    int N = 100;
    int t;
    while(i < N) {
        a[i] = N - i;
        i = i + 1;
    }

    while(j < N) {
        i = 0;
        while(i < N - 1) {
            if(a[i] > a[i + 1]) {
                t = a[i];
                a[i] = a[i + 1];
                a[i + 1] = t;
            }
            i = i + 1;
        }
        j = j + 1;
    }

    i = 0;
    while(i < N) {
        write(a[i]);
        i = i + 1;
    }

    return 0;
}
