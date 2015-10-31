struct A{
    int a[3][3][3];
    float b;
};

int main(){
    struct A k;
    int g = 0;
    k.a = g;
    k.a[1] = g;
    k.a[1][1] = g;
    k.a[1][1][1] = g;
    k.a[1][1][1][1] = g;
    return 0;
}
