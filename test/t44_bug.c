int main(){
    int a[8];
    int x,y;
    a[0] = 72;
    a[1] = 71;
    a[2] = 70;
    a[3] = 69;
    a[4] = 68;
    a[5] = 67;
    a[6] = 66;
    a[7] = 65;
    x = read();
    y = read();
    write(a[y]+x);
    write(a[y+x]);
    return 0;
}
