int main(){
    int a[2];
    int i = 0;
    int t;
    a[0] = 4;
    a[1] = 9;
    write(a[i]);
    write(a[i+1]);
    t = a[i];
    a[i] = a[i+1];
    a[i+1] = t;
    write(a[i]);
    write(a[i+1]);
    return 0;
}
