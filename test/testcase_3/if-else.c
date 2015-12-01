int if_else(int n) {
    int cost;
    if(n > 500) cost = 150;
    else if(n > 300) cost = 100;
    else if(n > 100) cost = 75;
    else if(n > 50) cost = 50;
    else cost = 0;

    return cost;
}

int main() {
    int i = 0;
    int test_data[128];
    int len = 32;

    while (i < len) {
        test_data[i] = read();
        write(if_else(test_data[i]));
        i = i + 1;
    }

    i = 0;
    while (i < len) {
        write(test_data[i]);
        i = i + 1;
    }
    return 0;
}
