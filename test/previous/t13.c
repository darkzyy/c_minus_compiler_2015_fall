int func(int a[2]){
    return 0;
}

int main(){
    int b[4][4];
    int a0 = func(b);
    int a1 = func(b[1]);
    int a2 = func(b[2][1]);
    return 1;
}
