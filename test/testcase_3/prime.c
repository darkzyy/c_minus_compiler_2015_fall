int mod(int a, int b) {
    return (a - (a / b) * b);
}

int main() {
    int m, i, n = 0;
    int prime;
    int ans[21];
    ans[0] = 101;
    ans[1] = 103;
    ans[2] = 107;
    ans[3] = 109;
    ans[4] = 113;
    ans[5] = 127;
    ans[6] = 131;
    ans[7] = 137;
    ans[8] = 139;
    ans[9] = 149;
    ans[10] = 151;
    ans[11] = 157;
    ans[12] = 163;
    ans[13] = 167;
    ans[14] = 173;
    ans[15] = 179;
    ans[16] = 181;
    ans[17] = 191;
    ans[18] = 193;
    ans[19] = 197;
    ans[20] = 199;
    m = 101;
    while (m <= 200) {
        prime = 1;

        i = 2;
        while (i < m) {
            if(mod(m, i) == 0) {
                prime = 0;
                i = m;
            }
            i = i + 1;
        }

        if(prime) {
            write(i == ans[n]);
            n = n + 1;
        }

        m = m + 2;
    }

    write(n == 21);

    return 0;
}

