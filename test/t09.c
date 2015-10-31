int a1,b1;
float a2,b2;
struct z{
    int x;
    float y;
}a3,b3;

int main(){
    int c1 = a1+b1;
    float c2 = a2+b2;
    c1 = a1+b1;
    c2 = a2+b2;
    c1 = c1+a3.x;
    c2 = c2+a3.y;
    c1 = c1+a3.y;
    c2 = c2+a3.x;
    return 0;
}

int n[5][4];
int func(int arg1,int arg2){
    int a = n[1][2]+1;
    return 0;
}

int func2(int arg3){
    return func(b3.x,func(n[3][4],func(c1,func2(main()))));
}

