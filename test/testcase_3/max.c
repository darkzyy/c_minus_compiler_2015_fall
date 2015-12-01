int max(int x, int y) {
    int z;
    if(x > y) { z = x; }
    else { z = y; }
    return z;
}

int main() {
    int i, j, ans_idx = 0;
    int NR_DATA = 8;
    int test_data[8];
    int ans[64];

    test_data[0] = 0;
    test_data[1] = 1;
    test_data[2] = 2;
    test_data[3] = 2147483647;
    test_data[4] = -2147483648;
    test_data[5] = -2147483647;
    test_data[6] = -2;
    test_data[7] = -1;
    ans[0] = 0;
    ans[1] = 1;
    ans[2] = 2;
    ans[3] = 2147483647;
    ans[4] = 0;
    ans[5] = 0;
    ans[6] = 0;
    ans[7] = 0;
    ans[8] = 1;
    ans[9] = 1;
    ans[10] = 2;
    ans[11] = 2147483647;
    ans[12] = 1;
    ans[13] = 1;
    ans[14] = 1;
    ans[15] = 1;
    ans[16] = 2;
    ans[17] = 2;
    ans[18] = 2;
    ans[19] = 2147483647;
    ans[20] = 2;
    ans[21] = 2;
    ans[22] = 2;
    ans[23] = 2;
    ans[24] = 2147483647;
    ans[25] = 2147483647;
    ans[26] = 2147483647;
    ans[27] = 2147483647;
    ans[28] = 2147483647;
    ans[29] = 2147483647;
    ans[30] = 2147483647;
    ans[31] = 2147483647;
    ans[32] = 0;
    ans[33] = 1;
    ans[34] = 2;
    ans[35] = 2147483647;
    ans[36] = -2147483648;
    ans[37] = -2147483647;
    ans[38] = -2;
    ans[39] = -1;
    ans[40] = 0;
    ans[41] = 1;
    ans[42] = 2;
    ans[43] = 2147483647;
    ans[44] = -2147483647;
    ans[45] = -2147483647;
    ans[46] = -2;
    ans[47] = -1;
    ans[48] = 0;
    ans[49] = 1;
    ans[50] = 2;
    ans[51] = 2147483647;
    ans[52] = -2;
    ans[53] = -2;
    ans[54] = -2;
    ans[55] = -1;
    ans[56] = 0;
    ans[57] = 1;
    ans[58] = 2;
    ans[59] = 2147483647;
    ans[60] = -1;
    ans[61] = -1;
    ans[62] = -1;
    ans[63] = -1;

    i = 0;
    while(i < NR_DATA) {
        j = 0;
        while(j < NR_DATA) {
            write(max(test_data[i], test_data[j]) == ans[ans_idx]);
            ans_idx = ans_idx + 1;
            j = j + 1;
        }
        i = i + 1;
    }

    return 0;
}
