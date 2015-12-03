struct A{
    int a;
    int b;
    int c[10];
};

int main(){
    struct A s;
    int aa[5];
    aa[4] = s.c[4] = aa[0] = s.a = 5;
    write(aa[4]);
    write(s.c[4]);
    write(aa[0]);
    write(s.a);
    return 0;
}
