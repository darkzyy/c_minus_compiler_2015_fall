#include <stdio.h>

void write(int data) {
    printf("%d\n", data);
}

int read() {
    int data;
    scanf("%d", &data);
    return data;
}

void print(int a[], int f, int e) {
    int i;
    for (i = f; i < e; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
}

void print_ans(int a[], int n, const char *name) {
    int i;
    for (i = 0; i < n; i++) printf("%s[%d] = %d;\n", name, i, a[i]);
}
