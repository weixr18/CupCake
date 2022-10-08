// string + macro

#include <stdio.h>
#define LEN 200

int main()
{
    printf("2333333333\n");
    int n, m, k;
    int A[LEN][LEN], B[LEN][LEN], C[LEN][LEN];
    scanf("%d %d %d", &n, &m, &k);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            scanf("%d", A+i*LEN+j);
        }
    }
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < k; j++)
        {
            scanf("%d", B+i*LEN+j);
        }
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < k; j++)
        {
            for (int t = 0; t < m; t++)
            {
                C[i][j] += A[i][t] * B[t][j];
            }
        }
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < k; j++)
        {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }
    return 0;
}