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
#define TARGET_HEIGHT 120
#define ycen 175  //y cordinate center of ellipse
#define xcen -79  // x cordinate center of ellipse
#define sr 65    // radius of smaller balloon
#define br 76   // radius of bigger radius
#define late 0  // delay time
#define NO_BU 500    /*currently this is the variable that controls how many bubble will be produced*/

struct screen
{
    int c[NO_BU];       // c for counters i.e i,j,k,l,m
    int asize[NO_BU];   //for storing the size (radius)of bubble in the screen
    int indi;     // indicates how many bullunes are on memory
    int saver1;     // for memory reuse and CPU efficency
    int color[NO_BU];
};
typedef struct screen s;

void score_keeper(int change)
{
    static int sc_n=0;       //score in numbers
    char score[101];
    sc_n += change;


    sprintf(score,"Score: %d",sc_n);
    settextstyle(4,0,1);
    setcolor(BLACK);
    outtextxy(getmaxx()-textwidth(score),textheight(score),score);
    setcolor(WHITE);
}
int check_w_b(s *s1)
{
    int i;
    for(i=(s1->saver1);i<(s1->indi);i++)
    {
        int t = (s1->asize[i]+(TARGET_WIDTH/2));      //temp -- for i.e radius of circle + screenwidth/2
        int oa = (xcen+s1->c[i]) - (SCREEN_WIDTH/2);
        if(oa<=t && oa>=(-t))
            return i;
       // return -1;          // i.e no bullunes lie at that position
    }
}
int rnd(int ll, int hl) // for generating random numbers from lower limit ll, to higher limit hl
{
    time_t raw_time = time(NULL);
    srand(raw_time);
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
    POINT m_po;        // declare a variable for mouse position
    int i;
    int b_t_r;        // is boolen variable for knowing if the mouse is witnin the target box
    static int page=0;
    setactivepage(page);
    setvisualpage(1-page);
    cleardevice();
    setlinestyle(0,0,2);
    for(i=(s1->saver1);i<=(s1->indi);i++)
    {
        setcolor(LIGHTBLUE);
        setfillstyle(1,LIGHTBLUE);
        fillellipse(xcen+s1->c[i],ycen,s1->asize[i],s1->asize[i]);     // bubbles
    }
    GetCursorPos(&m_po);
    setfillstyle(1,GREEN);  setcolor(GREEN);
    fillellipse(m_po.x,m_po.y,3,3);                             // mouse center pointer
    b_t_r = ((m_po.x)>=((SCREEN_WIDTH-TARGET_WIDTH)/2) && (m_po.x)<=((SCREEN_WIDTH+TARGET_WIDTH)/2) && (m_po.y)>=(ycen-TARGET_HEIGHT/2) && (m_po.y)<=(ycen+TARGET_HEIGHT/2));
    if(GetAsyncKeyState(VK_LBUTTON) && b_t_r)      // concidering leftclick for small bubble
    {
        int t_b = check_w_b(s1);                          //check_which_bubble lies in the target area and store it's i value(identifier) of bubble to t_b
        outtextxy(m_po.x,m_po.y,"Left Click at rSpot");
        if(s1->asize[t_b] == sr)                         // if the click is approprite
            score_keeper(10);                           //score up by 10
        else
            score_keeper(-10);                          // score down by 10
    }

    if(GetAsyncKeyState(VK_RBUTTON)&& b_t_r)
    {
        int t_b = check_w_b(s1);                          //check_which_bubble lies in the target area and store it's i value(identifier) of bubble to t_b
        if(s1->asize[t_b] == br)                         // if the click is approprite
            score_keeper(10);                           //score up by 10
        else
            score_keeper(-10);                          // score down by 10
        outtextxy(m_po.x,m_po.y,"Right Click at rSpot");

    }

    page = 1-page;
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
    int gd = DETECT, gm,i;
    //initgraph(&gd, &gm, "C:\\TC\\BGI");
    initwindow(SCREEN_WIDTH,SCREEN_HEIGHT,"Poploon");
    s s1;      // decalre a variable s1 of struct screen
    s1.saver1 = 0;
    int GAP;
    setlinestyle(SOLID_LINE,0,1);
    setbkcolor(WHITE);
    for(i=0;i<NO_BU;i++)
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
            target_box();
            score_keeper(0);            // display current score
            for(j=(s1.saver1);j<=i;j++)           //all the necessery increments for making all booluns moving
                s1.c[j]++;
            if((xcen+s1.c[s1.saver1])> (getmaxx()+br))
                (s1.saver1)++;
        }
    }
   getch();
   closegraph();
   return 0;
}



