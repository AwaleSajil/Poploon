#include <stdio.h>
#include <stdlib.h>
#include<graphics.h>
#include<conio.h>
#include<dos.h>
#include<time.h>

void main()

{
    int gd = DETECT, gm;
    initwindow(SCREEN_WIDTH,SCREEN_HEIGHT,TITLE);
    int i;

    for(i=0;;i++)
    {
        outtextxy(100+i,300+i,"Hello");
        delay(50);
    }
    getch();
}
