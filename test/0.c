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

// void func(int (a[3][4]), int n){
//     (a[n-1][0]) = 2;
//     n = (a[0][3]) + 2;
// }
