#include <stdio.h>
#include <stdlib.h>
//#include <math.h>
#include<graphics.h>
#include<conio.h>
#include<dos.h>
#include<time.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define TARGET_WIDTH 70
#define TARGET_HEIGHT 100
//#define GAP 156   // gap between consecutive baloons ,,, for consecutive big baloons= 180
#define ycen 175  //y cordinate center of ellipse
#define xcen -79  // x cordinate center of ellipse
#define sr 65    // radius of smaller balloon
#define br 76   // radius of bigger radius
#define late 0  // delay time
#define NO_BU 100    /*currently this is the variable that controls how many bubble will be produced*/

struct screen
{
    int c[NO_BU];       // c for counters i.e i,j,k,l,m
    int asize[NO_BU];   //for storing the size of bubble in the screen
    int indi:NO_BU;     // indicates how many bullunes are on memory + it's size is NO_BU bits ie can store 2^NO_BU ints value from 0 to 2^21(currently), currently this value should have been sqrt(NO_BU)
};
typedef struct screen s;

int rnd(int ll, int hl) // for generating random numbers from lower limit ll, to higher limit hl
{
    static int temp;
    //time_t raw_time = time(NULL);
    //srand(raw_time);
    time_t raw_time;
    for(;temp>0;temp--)
    {
        if(temp>1)
            raw_time = time(NULL)%temp + time(NULL);
        else if(temp>2)
            raw_time = time(NULL)+temp++-temp;
        else
        {
            if(temp%2==0)
                continue;
            break;
        }

            temp++;
        srand(raw_time);
    }
    return (rand()%(hl-ll+1) + ll);
}

int rd()
{
    if(rnd(0,1))
        return br;
    else
        return sr;
}

void bu_gen(s *s1)
{
    int i;
    setfillstyle(1,LIGHTBLUE);
    for(i=0;i<=(s1->indi);i++)
           fillellipse(xcen+s1->c[i],ycen,s1->asize[i],s1->asize[i]);
    delay(late);
}
void target_box()
{
    setcolor(RED);
    setlinestyle(0,0,5);
    rectangle((SCREEN_WIDTH-TARGET_WIDTH)/2, ycen-TARGET_HEIGHT/2,(SCREEN_WIDTH+TARGET_WIDTH)/2,ycen+TARGET_HEIGHT/2);
    setlinestyle(1,0,2);
    line(SCREEN_WIDTH/2,ycen-TARGET_HEIGHT/2,SCREEN_WIDTH/2,ycen+TARGET_HEIGHT/2);
    line(SCREEN_WIDTH/2-TARGET_WIDTH/2,ycen,SCREEN_WIDTH/2+TARGET_WIDTH/2,ycen);
}
int main()
{
    POINT m_po;        // declare a variable for mouse position
    int sc=800;
    char score[101];
    int gd = DETECT, gm,i;
    //initgraph(&gd, &gm, "C:\\TC\\BGI");
    initwindow(SCREEN_WIDTH,SCREEN_HEIGHT,"Poploon");
    s s1;      // decalre a variable s1 of struct screen
    int GAP;
    setlinestyle(SOLID_LINE,0,1);
    setbkcolor(WHITE);
    for(i=0;;i++)
    {
        if(i==0)
            s1.asize[i]=rd();
        for(s1.c[i]=1,s1.asize[i+1]=rd(),s1.indi=i;;)     //i is the identifier of each bubblne here
        {
            int j;
            if(s1.c[i]==1)
            {
                if(s1.asize[i] == s1.asize[i+1])   // if consecutive bubble are of same size
                {
                    if(s1.asize[i] == sr)
                        GAP = rnd(2*sr, 3*sr);       // max gap between consecutive small bubble is 1.5 times min GAP
                    else                            //if(s1.asize[i] == br)
                        GAP = rnd(2*br, 3*br);       // max gap between consecutive big bubble is 1.5 times min GAP
                }
                else
                {
                    GAP = rnd(sr+br, 2*sr+br);      // max limit is approx 1.5 times min vlaue for current value
                }

            }
            if(s1.c[i]%GAP==0)      //c for counter
                break;
            bu_gen(&s1);
            GetCursorPos(&m_po);

            circle(m_po.x,m_po.y,50);
            target_box();
            if(GetAsyncKeyState(VK_LBUTTON))
                rectangle(m_po.x,m_po.y,m_po.x+100,m_po.y+100);
            if(GetAsyncKeyState(VK_RBUTTON))
                bar(m_po.x,m_po.y,m_po.x+50,m_po.y+50);

            sprintf(score,"Score: %d",sc);
            settextstyle(4,0,1);
            setcolor(BLACK);
            outtextxy(getmaxx()-textwidth(score),textheight(score),score);
            setcolor(WHITE);
            for(j=0;j<=i;j++)           //all the necessery increments for making all booluns moving
                s1.c[j]++;
        }
    }
   getch();
   closegraph();
   return 0;
}


