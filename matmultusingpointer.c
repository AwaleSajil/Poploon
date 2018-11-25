/*
//matrix addition by pointer by sajil'sstyle
#include <stdio.h>
void input(int *mat,int m,int n)
{
    int i;
    for(i=0;i<(m*n);i++)
        scanf("%d",&mat[i]);
}
void process(int *mata,int *matb,int *matc,int m,int n)
{
    int i;
    for(i=0;i<(m*n);i++)
        matc[i] = mata[i] + matb[i];
}
void display(int *mat, int m, int n)
{
    int i,j;
    for(i=0;i<m;i++)
    {
        for(j=0;j<n;j++)
        {
            static k = 0;
            printf("%d ",mat[k]);
            k = k +1;
        }
        puts("");
    }
}
void main()
{
    int m,n;
    puts("Enter order of matrix");
    fflush(stdin);
    scanf("%d %d",&m,&n);
    {
        int mata[m][n], matb[m][n], matc[m][n];
        puts("Enter 1st matrix");
        input(mata,m,n);
        puts("Enter 2nd matrix");
        input(matb,m,n);
        process(mata, matb,matc, m, n);
        display(matc,m,n);
    }

    fflush(stdin);
    getchar();
}
*/
#include <stdio.h>
void input(int *mat,int m,int n)
{
    int i;
    for(i=0;i<(m*n);i++)
        scanf("%d",&mat[i]);
}
void process(int *m1,int *m2,int *m3,int m,int q)
{
    int i,j,n;
    for(i=0;i<m;i++)
    {
        for(j=0;j<q;j++)
        {
            m3[i+j] =0;
            for(n=0;n<q;n++)
                m3[i+j] += m1[i+n] * m2[n+j];
        }

    }
}
void display(int *mat, int m, int n)
{
    int i,j;
    for(i=0;i<m;i++)
    {
        for(j=0;j<n;j++)
        {
            static k = 0;
            printf("%d ",mat[k]);
            k = k +1;
        }
        puts("");
    }
}
void main()
{
    int m,n;

    puts("Enter order of matrix(again)");
    fflush(stdin);
    scanf("%d %d",&m,&n);
    {
        int mata[m][n], matb[m][n], matc[m][n];
        puts("Enter 1st matrix");
        input(mata,m,n);
        puts("Enter 2nd matrix");
        input(matb,m,n);
        process(mata, matb,matc, m, n);
        display(matc,m,n);
    }

    fflush(stdin);
    getchar();
}
