#include <stdio.h>
#include <stdlib.h>

int main()
{
    int *ptr[2000],i;
    for(i=0;i<1000;i++)
    {
        *ptr[i] = malloc(2);
        if(ptr[i] == NULL)
            puts("malloc failed");
    }

    puts("Enter '1' key");
    fflush(stdin);
    scanf("%d",ptr[0]);

    if(ptr[0] == 1)
    {
        for(i=0;i<1000;i++)
            free(ptr[i]);
    }

        free(ptr);

    scanf("%d",&ptr[1]);
    fflush(stdin);
    getchar();
    return 0;
}
