// basic

double baala(){
    double pi = 3.14159;
    double r = 464.2;
    double h = 5;
    double s1,s2,s;
    s1 = pi*r*r;
    s2 = 2*pi*r*h;
    s = s1*2.0+s2;
    return s;
}

int pr(int n){
    // prime number
    int k; 
    int len = sqrt(n);
    for (k=2; k <= len; k++){
        if (n % k == 0){
            return 0; 
        }
    }
    return 1; 
} 


int zebra = 654;

int sumPrime(void){ 
    int i = 2;
    long sum = 0; 
    while(i <= 100){
        if(pr(i)){
            sum += i;
        }
        i++;
    }
    return sum; 
} 


int leftOf(double x, double y, double a, double b)
{
    double cross = (a*b) - (a*y) - (b*x);
    if (cross > 0){
        return -1;
    }   
    else {
        if (cross < 0){
            return 1;
        }        
        else{
            return 0;
        }
    }
}