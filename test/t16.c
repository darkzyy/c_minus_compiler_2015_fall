struct A{
    int a;
    int b,c,cc,ccc;
    int d;
    float e;
};
struct B{
    struct A f;
    int g;
    int h;
    struct A i,k;
};
struct C{
    int s0,s1;
};
int main(){
    struct A AA;
    AA.cc = 0;
}
