struct A {
    int a1,a2,a3,a4,a5[5];
};
struct B{
    int b1,b2;
    struct A b3,b4[5];
    int b5;
};

struct C{
    struct B c1,c2;
    struct A c3;
    int c4,c5;
};

int main(){
    struct C n;
    int x;
    x = read();
    n.c2.b4[4].a5[0] = 4;
    n.c1.b4[0].a4 = 4;
    n.c1.b1 = 7;
    n.c3.a5[4] = 7;
    n.c2.b4[x].a5[0] = 10;
    write((n.c2.b4[4].a5[0]!=n.c1.b4[0].a4)==(n.c1.b1!=n.c3.a5[4]));
    return 0;
}
