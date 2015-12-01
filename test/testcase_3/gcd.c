int mod(int aaa, int bbb) {
    return (aaa - (aaa / bbb) * bbb);
}

int gcd_1(int m, int n) {
    if (n == 0) return m;
    else {
        return gcd_1(n, mod(m, n));
    }
}

int gcd_2(int x, int y) {
    int t;
    while (y != 0) {
        t = y;
        y = mod(x, y);
        x = t;
    }
    return x;
}

int main() {
    int i = read();
    int xx, yy;
    while (i != 0) {
        xx = read();
        yy = read();
        write(gcd_1(xx, yy));
        write(gcd_2(xx, yy));
        i = read();
    }
    return 0;
}
