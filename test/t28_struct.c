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
    aa.a = 1;
    return aa.a;
}
