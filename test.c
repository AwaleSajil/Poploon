#include <stdio.h>
void main()
{
    FILE *fp;
    char str[3][40];
    int i,k=0;
    fp=fopen("stringc.txt", "w");
    if(fp==NULL)
    {
        printf("file coulnd not be opened");
        exit(1);
    }

    for(i=0;i<3;i++)
    {
        puts("enter text");
        fflush(stdin);
        scanf("%[^\n]", str[i]);
        fputs(str[i],fp);

    }
    fclose(fp);


    fp=fopen("stringc.txt", "r");
    for(i=0; i<3; i++)
    {
         while(fgets(str[i], 40, fp)!=NULL)
      puts(str[i]);
    }

    fclose(fp);
    getchar();

}
