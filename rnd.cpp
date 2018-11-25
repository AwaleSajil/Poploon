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
#define CY_RANGE 5
#define CX_RANGE 2
#define RNX_BUBB 5
unsigned long ud_rnd()
{
    static unsigned long a=123456879, b=356789879, c=578762099;
    unsigned long t;
    a ^= a << 13;
    a ^= a >> 17;
    a ^= a << 5;
    t = a;
    a = b;
    b = c;
    c = t ^ a ^ b;
    return c;
}

int rnd(short int ll,short int hl) // for generating random numbers from lower limit ll, to higher limit hl
{
    unsigned int raw_t = ((int)time(NULL) + abs(ud_rnd()));
    srand(raw_t);
    return (rand()%(hl-ll+1) + ll);
}

int main()
{
    for(;;)
    {
        printf("%d\t",rnd(-5,5));
        delay(50);
    }
    fflush(stdin);
    getchar();
    return 0;
}
