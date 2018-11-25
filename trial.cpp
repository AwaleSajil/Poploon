#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <conio.h>
#include <dos.h>
#include <time.h>
#include <math.h>
#define TITLE "Pooploon"
#define SCREEN_WIDTH 800                //1360
#define SCREEN_HEIGHT 600               //700
#define TARGET_HEIGHT 500
#define xcen -79  // x cordinate center of ellipse
#define sr 65    // radius of smaller balloon
#define br 85   // radius of bigger radius
#define NO_BU 1000   /*currently this is the variable that controls how many bubble will be produced*/
#define H_S_LOCATION "HighScore.dat"
#define B_DEL 400
#define INF_DEL 300
#define TOP_SC 10
#define FRAME_DEL 50  //about 20 fps
#define CY_RANGE 15
#define CX_RANGE 2



int main()
{
    initwindow(SCREEN_WIDTH,SCREEN_HEIGHT,TITLE,true,true);
    for(;;)
    {
        if(ismouseclick(WM_LBUTTONUP))
        {
            printf("YO  ");
            clearmouseclick(WM_LBUTTONUP);
        }

        delay(1);
    }

    return 0;
}
