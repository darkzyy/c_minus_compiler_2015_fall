struct A{
    int a;
    float b;
    int c[10];
    float d[10];
};

struct B{
    struct A e[10];
    int f;
    struct A g;
};

struct B h[10];

struct C{
    int i;
    struct B j[10];
    int k;
};
struct C l[10];

int func1(float arg_f){
    return 0;
}

float func2(int arg_i){
    return 0.0;
}

int main(){
    if(func2(l[0].j[0].g.d[9]) == l[1].j.e[3].b){
        h[0].f =0;
        h[0].g.b=0.0;
    }
    return 0;
}
