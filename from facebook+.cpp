#include <stdio.h>
#include<graphics.h>
#include<conio.h>
#include<dos.h>
#include<time.h>
#define GAP 120   // gap between consecutive baloon
#define ycen 65  //y cordinate center of ellipse
#define xcen 79  // x cordinate center of ellipse
#define sr 56    // radius of smaller balloon
#define br 89   // radius of bigger radius
#define late 21  // delay time

int main()
{
    int gd = DETECT, gm,i,j, k, l, m;
    initgraph(&gd, &gm, "C:\\TC\\BGI");
    int asize[7];       //for storing the size of bubble in the screen
    setbkcolor(WHITE);
    setcolor(BLUE);


    for(i=1;;i++)
        if(i%GAP==0)
            break;
    getch();
    closegraph();
    return 0;
}

