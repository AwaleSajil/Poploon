#include <stdio.h>
#include <stdlib.h>
#include<graphics.h>
#include<conio.h>
#include<dos.h>
#include<time.h>

int main()

{
    int gd = DETECT, gm;
    initwindow(1000,600,"jojo");
    int i;

    for(i=0;;i++)
    {
        outtextxy(100+i,300+i,"Hello");
        line(200+i,100+i,500+i,400+i);
        delay(50);
    }
    getch();
    return 0;
}
