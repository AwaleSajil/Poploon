#include<stdio.h>
int main(){
char z[20];
int a, i;
printf("Enter a number.\n");
//scanf("%d",&a);
fflush(stdin);
for (i=0;i<20;i++){
    z[i]=0;
}
fflush(stdin);
a=getch()-48;
if (z[a]){
z[a]='N';}
printf("%c\n %c\n %c\n %c\n",z[1],z[2],z[3],z[4]);
}
