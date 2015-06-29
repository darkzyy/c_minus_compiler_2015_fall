struct A{
    int a;
};
struct A b[10];

int func(int arg_a){
    return 0;
}

int main(){
    func(b[0].a);
    return 0;
}
