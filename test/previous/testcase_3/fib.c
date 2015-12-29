int fib(int x) {
    if (x == 1 || x == 0) {
        return 1;
    } else {
        return fib(x - 1) + fib(x - 2);
    }
}

int main() {
    write(fib(10));
    return 0;
}
