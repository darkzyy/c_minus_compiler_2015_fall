int main(){
    int a[3];
    int i = 0;
    int b;
    while(i<3){
        a[i] = i;
        i = i+1;
    }
    i = 0;
    while(i<3){
        b = a[i];
        write(b);
        i = i+1;
    }
    return 0;
}
