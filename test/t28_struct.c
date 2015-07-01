struct A{
    int a;
    float b;
};
struct B{
    int c;
    struct A s;
    int k,l;
};
int main(){
    struct A aa;
    struct B bb;
    aa.a = 1;
    write(aa.a);
    bb.k = 2;
    write(bb.k);
    bb.s.a=3;
    write(bb.s.a);
    return aa.a;
}
