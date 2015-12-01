int mod(int aaa, int bbb) {
    return (aaa - (aaa / bbb) * bbb);
}

int is_prime(int n1) {
    int ii;

    if(n1 < 2) return 0;

    ii = 2;
    while (ii < n1) {
        if (mod(n1, ii) == 0) {
            return 0;
        }
        ii = ii + 1;
    }

    return 1;
}

int gotbaha(int n2) {
    int k;
    k = 2;
    while (k < n2) {
        if(is_prime(k) && is_prime(n2 - k)) {
            return 1;
        }
        k = k + 1;
    }
    return 0;
}

int main() {
    int n3;
    n3 = 4;
    while (n3 <= 100) {
        write(gotbaha(n3) == 1);
        n3 = n3 + 2;
    }

    return 0;
}
