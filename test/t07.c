int adda(int a, int b){
    return a+b;
}

int main(){
    int c = 10;
    int d = 20;
    int x[3];
    x[0] = 9;
    x[1] = 10;
    write(adda(x[0],x[1]));
    write(adda(c,d));
    return 0;
}
