//int global_variable=10;
int func(int a, int b) {
    int sum=0;
    sum=a+b;
    if(sum%2==0){
        sum=sum*10;
    } else {
        sum=sum*3;
    }
    return sum;
}