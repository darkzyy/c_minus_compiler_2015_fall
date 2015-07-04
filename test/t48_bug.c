int main(){
    int a[4];
    int x;
    x = read();
    a[x] = 2;
    x = read();
    a[a[0]+0] = 3;
    a[x] = 3;
    write(a[a[0]+1]);
    return 0;
}
