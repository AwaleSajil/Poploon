#include <stdio.h>
#include <stdlib.h>
void main()
{
    int i,a,b,rad;
    srand(time(NULL));
    fputs("Rnter the range of random numbers",stdout);
    fflush(stdin);
    scanf("%d %d",&a,&b);
    for(i=0;i<20;i++)
    {
        if(a>b)
            rad = rand()%(a-b+1)+b;
        else
            rad = rand()%(b-a+1)+a;
        printf("Random numer is %d\n",rad);
    }
    getch();
}
