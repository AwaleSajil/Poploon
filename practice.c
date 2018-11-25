//1
#include <stdio.h>
void main()
{
    char buf,buf2,str[41];
    FILE *fptr1 = fopen("xt.txt","w");
    FILE *fptr2  = fopen("copt_xt.txt","w");
    (fptr1 == NULL) ? exit(1): "";
    (fptr2 == NULL) ? exit(1): "";

    puts("Enter a string to store in file");
    fflush(stdin);
    scanf("%[^\n]",str);

    fputs(str,fptr1);

    fclose(fptr1);

    fptr1 = fopen("xt.txt","r");
    (fptr1 == NULL) ? exit(1): "";

    while( (buf = fgetc(fptr1)) != EOF)
    {
        fputc(buf,fptr2);
    }

    fclose(fptr2);

    fptr2  = fopen("copt_xt.txt","r");
    (fptr2 == NULL) ? exit(1): "";

    puts("from file copied");
    while((buf2 = fgetc(fptr2))!= EOF)
        putchar(buf2);
    fclose(fptr1);
    fclose(fptr2);
    fflush(stdin);
    getchar();
}
