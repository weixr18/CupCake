// array

void BubbleSort(int (a[10]), int n){
	int i, j, t;
    int flag = 0;
	for(i = 0; i < n; i++){
		for(j = n-1; j > i; j--){
			if((a[j-1]) > a[j]){
				t = a[j-1];
				(a[j-1]) = a[j];
				(a[j]) = t;
				flag = 1;
			}
		}
		if(flag == 0)
		    break;
		else {
            flag = 0;
        }		
	}
 }

void MergeStep(int (a[1]), int (r[1]), int (s[1]), int m, int n){
    int i, j, k;
    k = s;
    i = s;
    j = m + 1;
    while(i <= m && j <= n) {
		if((a[i]) <= a[j]) {
			(r[k++]) = a[i++];
		}
        else{
			(r[k++]) = a[j++];
		}
    }
    while(i <= m){
        (r[k++]) = a[i++];
	}
    while(j <= n){
		(r[k++]) = a[j++];
	}
}

void MergePass(int (a[1]), int (r[1]), int n, int len) 
{
    int e;
    int s = 0;           
    while((s+len) < n) {
        e = s + (2*len) - 1;   
        if(e >= n) 
            e = n-1; 
        MergeStep(a, r, s, (s+len-1), e); 
        s = e + 1;  
    }
    if(s < n) 
        for(; s<n ; s++){
			(r[s]) = a[s];
		}
            
}