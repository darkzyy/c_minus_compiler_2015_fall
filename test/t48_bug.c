int main(){
    int a[4];
    int x;
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;
    a[3] = 4;
    x = read();
    a[x] = 2;
    x = read();
    a[a[0]+0] = 3;
    a[x] = 3;
    write(a[a[0]+1]);
    return 0;
}
