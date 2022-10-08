// pointer


int binSearch(int *ends, double lo, double hi, double x, double y)
{
    if (lo + 1 >= hi)
        return lo;

    int mi = (lo + hi) / 2;
    int res = leftOf(x, y, ends[2 * mi], ends[2 * mi + 1]);
    if (res < 0)
        return binSearch(ends, lo, mi, x, y);
    else{
        if (res > 0)
            return binSearch(ends, mi, hi, x, y);
        else{
            return mi;
        }
    } 
}


// void MergeSort(int a[],int n)   
// {
//     int *p;
//     int len=1;     
//     int f=0;	
//     if(!(p=(int *)malloc(sizeof(int)*n)))	
//     {
//         // mem alloc failed!
//         exit(0); 
//     }
//     while(len<n)
//     {
//         if(f)   
//             MergePass(p,a,n,len);	
//         else
//             MergePass(a,p,n,len);	
//         len*=2;
//         f=1-f;  
//     }
//     if(f){
//         for(f=0;f<n;f++)	
//             a[f]=p[f];
//     }
//     free(p); 
// }
