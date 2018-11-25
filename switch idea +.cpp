#include <stdio.h>
#include <stdlib.h>
//#include <math.h>
#include<graphics.h>
#include<conio.h>
#include<dos.h>
#include<time.h>
#define GAP 180   // gap between consecutive baloons ,,, for consecutive big baloons= 180
#define ycen 105  //y cordinate center of ellipse
#define xcen -79  // x cordinate center of ellipse
#define sr 56    // radius of smaller balloon
#define br 89   // radius of bigger radius
#define late 0  // delay time
#define NO_BU 100    /*currently this is the variable that controls how many bubble will be produced*/

struct screen
{
    int c[NO_BU];       // c for counters i.e i,j,k,l,m
    int asize[NO_BU];   //for storing the size of bubble in the screen
    int indi:NO_BU;     // indicates which bubbles are selected(animated) at that instant + it's size is NO_BU bits ie can store 2^NO_BU ints value from 0 to 2^21(currently), currently this value should have been sqrt(NO_BU)
};
typedef struct screen s;

int rnd(int higher_limit) //for random number generator
{
    static int temp;
    //time_t raw_time = time(NULL);
    //srand(raw_time);
    time_t raw_time;
    for(;temp>0;temp--)
    {
        if(temp>1)
        raw_time = time(NULL)%temp;
    else
        raw_time = time(NULL)+temp++-temp;
    temp++;
    srand(raw_time);
    }

    return ((rand()%higher_limit));
}
int rd()
{
    if(rnd(2))
        return br;
    else
        return sr;
}

void bu_gen(s *s1)
{
    int i;
    for(i=0;i<=(s1->indi);i++)
        fillellipse(xcen+s1->c[i],ycen,s1->asize[i],s1->asize[i]);
    delay(late);
}

int main()
{
    int gd = DETECT, gm,i;
    //initgraph(&gd, &gm, "C:\\TC\\BGI");
    initwindow(800,600,"Poploon");
    s s1;      // decalre a variable s1 of struct screen
    setbkcolor(WHITE);
    setcolor(BLUE);
    for(i=0;;i++)
    {
        for(s1.c[i]=1,s1.asize[i]=rd(),s1.indi=i;;)
        {
            int j;
            if(s1.c[i]%GAP==0)
            {
                break;
            }
            bu_gen(&s1);
            for(j=0;j<=i;j++)           //all the necessery increments for making all booluns moving
                s1.c[j]++;
        }
    }
   getch();
   closegraph();
   return 0;
}

