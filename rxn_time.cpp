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



#define RNX_BUBB 5
unsigned long ud_rnd()
{
    static unsigned int a=126873519, b=398209817, c=578298231;
    unsigned long t;
    a ^= a << 16;
    a ^= a >> 5;
    a ^= a << 1;
    t = a;
    a = b;
    b = c;
    c = t ^ a ^ b;
    return c;
}
int rnd(short int ll,short int hl) // for generating random numbers from lower limit ll, to higher limit hl
{
    unsigned int raw_t = (time(NULL) + abs(ud_rnd()/100));
    srand(raw_t);
    return (rand()%(hl-ll+1) + ll);
}

void b_but(short int x, short int y, char str[], char color)
{
    setcolor(color);
    outtextxy(x,y,str);
}
int main()
{
    char mess[21] = "OUT OUT !!!";
    char ins[] = "Click bullble as soon as bubble gets filled";
    short int sum= 0,fill_color;
    initwindow(SCREEN_WIDTH,SCREEN_HEIGHT,TITLE,true,true);
    floodfill(SCREEN_WIDTH-1,SCREEN_HEIGHT-1,WHITE);
    setbkcolor(WHITE);  settextstyle(8,0,3);
    b_but(SCREEN_WIDTH/2-textwidth(ins)/2,SCREEN_HEIGHT/5,ins,0);
    for(int j=0;j<RNX_BUBB;j++)
    {
        setcolor(1);    setfillstyle(1,15);
        fillellipse(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,br,br);
        fill_color = rnd(0,14);
        while((GetAsyncKeyState(VK_LBUTTON)))   delay(20);
        delay(rnd(1000,5000));
        if((GetAsyncKeyState(VK_LBUTTON)))
        {
            //out out
            setbkcolor(15);
            settextstyle(3,0,5);
            b_but(SCREEN_WIDTH/2 - textwidth(mess)/2,SCREEN_HEIGHT/2 - textheight(mess)/2, mess, 0);
            delay(3000);
            return 0;
        }
            int i;
            for(i=0;;i++)
            {
                setcolor(fill_color);
                setfillstyle(1,fill_color);       //COLOR(R,G,B)
                fillellipse(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,br,br);// bubbles
                if(GetAsyncKeyState(VK_LBUTTON))
                {
                    if((sqrt(pow((SCREEN_WIDTH/2) - (mousex()),2) + pow((SCREEN_HEIGHT/2) - (mousey()),2))) < br)
                            break;
                }
                delay(1);
            }
            sum += i;
    }
    sprintf(mess,"Rxn time %.2f ms",(float)sum/RNX_BUBB);
    setbkcolor(fill_color);
    settextstyle(8,0,5);
    b_but(SCREEN_WIDTH/2 - textwidth(mess)/2,SCREEN_HEIGHT/2 - textheight(mess)/2, mess, 15);
    delay(3000);
    closegraph();
    return 0;
}
