#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <conio.h>
#include <dos.h>
int main()
{
    int gd = DETECT, gm;
    for(int i=0;;i++)
    {
        initwindow(500,500,"vc",true,true);
        line(100,100+i,100+i,200);
        closegraph();
        delay(5);
    }
    return 0;
}
