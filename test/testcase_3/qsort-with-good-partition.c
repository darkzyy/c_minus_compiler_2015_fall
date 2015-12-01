int partition(int array[1], int first, int last) {
    int pivot = array[first];
    int part_i = first;
    int tmp;

    int part_j = first + 1;
    while (part_j < last) {
        if (array[part_j] <= pivot) {
            part_i = part_i + 1;

            tmp = array[part_i];
            array[part_i] = array[part_j];
            array[part_j] = tmp;
        }

        part_j = part_j + 1;
    }

    tmp = array[part_i];
    array[part_i] = array[first];
    array[first] = tmp;
    return part_i;
}

int qsort(int A[1], int beg, int end) {
    int pivot_index;
    int temp;

    if (beg == end - 2) {
        if (A[beg] > A[end - 1]) {
            temp = A[beg];
            A[beg] = A[end - 1];
            A[end - 1] = temp;
        }
        return 0;
    } else if (beg > end - 2) {
        return 0;
    }

    pivot_index = partition(A, beg, end);
    qsort(A, beg, pivot_index + 1);
    qsort(A, pivot_index + 1, end);
    return 0;
}

int main() {
    int input;
    int nr = 0;
    int idx = 0;
    int list[128];

    while (nr < 128) {
        list[nr] = 128 - nr;
        nr = nr + 1;
    }

    qsort(list, 0, nr);

    while (idx < nr) {
        write(list[idx]);
        idx = idx + 1;
    }
    return 0;
}
