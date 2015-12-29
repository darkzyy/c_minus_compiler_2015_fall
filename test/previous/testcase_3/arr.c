int main() {
    int size = 10;
    int array[10];
    int i = 0, j = 0, k = 0;
    array[0] = 0;
    array[1] = 1;
    array[2] = 2;
    array[3] = 3;
    array[4] = 4;
    array[5] = 5;
    array[6] = 6;
    array[7] = 7;
    array[8] = 8;
    array[9] = 9;
    while (i < size) {
        while (j < size) {
            while (k < size) {
                write(array[array[array[k]]]);
                k = k + 1;
            }
            j = j + 1;
        }
        i = i + 1;
    }
    return 0;
}
