#include <stdio.h>
typedef struct info
{
    int rol,age,m[3];
    char name[101],add[101];
}inf;

void main()
{
    inf inf1;
    int i;
    FILE *fptr = fopen("d:\\std.txt","w");
    if(fptr == NULL)
    {
        perror("Couldnot open -");
        exit(1);
    }
    for(i=0;i<3;i++)
    {
        puts("Enter your name, address roll and age");
        fflush(stdin);
        scanf("%s %s %d %d",inf1.name,inf1.add,&inf1.rol,&inf1.age);
        puts("marks in 3 subject (physics, C, maths)");
        fflush(stdin);
        scanf("%d %d %d",&inf1.m[0],&inf1.m[1],&inf1.m[2]);
        fwrite(&inf1,sizeof(inf1),1,fptr);
    }
    fclose(fptr);
    fptr = fopen("d:\\std.txt","r");
    if(fptr == NULL)
    {
        perror("Couldnot open -");
        exit(1);
    }
    for(i=0;i<3;i++)
    {
        fread(&inf1,sizeof(inf1),1,fptr);
        printf("Name: %s Address: %s Roll: %d Age:%d AveMark:%f\n",inf1.name,inf1.add,inf1.rol,inf1.age,(inf1.m[0]+inf1.m[1]+inf1.m[2])/3.0);
    }
    fclose(fptr);
}
