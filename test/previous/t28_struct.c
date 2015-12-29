struct A{
    int a,b;
};
struct B{
    int c;
    struct A s;
    int k,l;
};
int main(){
    struct A aa;
    struct B bb;
    aa.b = 1;
    bb.k = 2;
    bb.s.a=3;
    write(aa.b);
    write(bb.k);
    write(bb.s.a);
    return aa.a;
}
