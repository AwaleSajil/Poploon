#include <stdio.h>
#include <stdlib.h>
//#include <math.h>
#include<graphics.h>
#include<conio.h>
#include<dos.h>
#include<time.h>
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define TARGET_WIDTH 2
#define oTARGET_WIDTH 71
#define TARGET_HEIGHT 120
#define ycen 175  //y cordinate center of ellipse
#define xcen -79  // x cordinate center of ellipse
#define sr 65    // radius of smaller balloon
#define br 85   // radius of bigger radius
//#define late 5  // delay time
#define NO_BU 500    /*currently this is the variable that controls how many bubble will be produced*/
#define UP_SCORE 1

struct screen
{
    int c;       // c for counters i.e i,j,k,l,m
    int asize;   //for storing the size (radius)of bubble in the screen
    int color;
};
struct addtional_info
{
    int indi;     // indicates how many bullunes are on memory
    int saver1;     // for CPU efficency
    int ssh;        // ssh --> stop scoring high
};
typedef struct screen s;
typedef struct addtional_info inf;
void over(int score,char *reason)
{
    char g_over[] = "GAME OVER !!!";
    char sc[15];
    setbkcolor(BLACK);
    settextstyle(3,0,8);
    outtextxy((getmaxx()/2)-(textwidth(g_over)/2),(getmaxy()/2)-(textheight(g_over)/2),g_over);
    sprintf(sc,"Score is %d",score);
    settextstyle(3,0,2);
    outtextxy((getmaxx()/2)-(textwidth(reason)/2),(getmaxy()/2)-3*(textheight(reason)),reason);
    settextstyle(3,0,5);
    outtextxy((getmaxx()/2)-(textwidth(sc)/2),(getmaxy()/2)+(textheight(sc)/1.5),sc);
    delay(5000);
    exit(0);
}
int score_keeper(int change)
{
    static int sc_n=0;       //score in numbers
    char score[101];
    sc_n += change;
    sprintf(score,"Score: %d",sc_n);
    settextstyle(4,0,4);
    setcolor(BLACK);
    settextstyle(3,0,3);
    outtextxy(getmaxx()-textwidth(score),textheight(score),score);
    setcolor(WHITE);
    return sc_n;
}
int check_w_b(s *s1, inf *inf1)
{
    int i;
    static int prev_i;
    for(i=(inf1->saver1);i<(inf1->indi);i++)
    {
        int t = (s1[i].asize+(TARGET_WIDTH/2));      //temp -- for i.e radius of circle + screenwidth/2
        int oa = (xcen+s1[i].c) - (SCREEN_WIDTH/2);
        if(oa<=t && oa>=(-t))
        {
            if(prev_i != i)
                inf1->ssh = 0;      //reset the sopt score high if new bullune is on the target
            return (prev_i=i);
        }

    }
    return -1;          // i.e no bullunes lie at that position
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
void late_process(int *late, int score)
{
    if(score<3)
        *late = 4;
    else if(score>2 && score<5)
        *late = 3;
    else if (score>4 && score<7)
        *late = 2;
    else if (score>6 && score<9)
        *late = 1;
    else if (score>8 && score<11)
        *late = 0;
    else
        *late = rnd(0,5);
}
void bu_gen(s *s1,inf *inf1)
{
    POINT m_po;        // declare a variable for mouse position
    int i;
    int b_t_r;        // is boolen variable for knowing if the mouse is witnin the target box
    int o_o_w; //bool to know if mouse is out of game window
    static int page=0, late = 4;
    setactivepage(page);
    setvisualpage(1-page);
    cleardevice();
    setlinestyle(0,0,2);
    for(i=(inf1->saver1);i<=(inf1->indi);i++)
    {
        setcolor(WHITE);
        setfillstyle(1,s1[i].color);
        fillellipse(xcen+s1[i].c,ycen,s1[i].asize,s1[i].asize);// bubbles
        setcolor(s1[i].color);
        line(xcen+s1[i].c,ycen+s1[i].asize,xcen+s1[i].c,ycen+s1[i].asize+sr*1.5); //string of bubble
    }
    GetCursorPos(&m_po);
    setfillstyle(1,WHITE);  setcolor(RED);
    fillellipse(m_po.x,m_po.y,3,5);                             // mouse center pointer
    b_t_r = ((m_po.x)>=((SCREEN_WIDTH-oTARGET_WIDTH)/2) && (m_po.x)<=((SCREEN_WIDTH+oTARGET_WIDTH)/2) && (m_po.y)>=(ycen-TARGET_HEIGHT/2) && (m_po.y)<=(ycen+TARGET_HEIGHT/2));
   //  o_o_w = ||
    int t_b = check_w_b(s1,inf1); //check_which_bubble lies in the target area and store it's i value(identifier) of bubble to t_b

    if((GetAsyncKeyState(VK_LBUTTON) || GetAsyncKeyState(VK_RBUTTON))&&((inf1->ssh)++) == 0)
    {
        if (t_b == -1)
        {
            setvisualpage(page);
            cleardevice();
            over(score_keeper(0),"You Clicked Where there is no Ballon");
        }

        if(GetAsyncKeyState(VK_LBUTTON)&& s1[t_b].asize == sr && b_t_r)
            score_keeper(UP_SCORE);
        else if(GetAsyncKeyState(VK_RBUTTON) && s1[t_b].asize == br && b_t_r)
            score_keeper(UP_SCORE);
        else
            {
                setvisualpage(page);
                cleardevice();
                over(score_keeper(0),"Wrong Click");
            }
    }
    page = 1-page;
    late_process(&late,score_keeper(0));
    delay(late);

}
void target_box()
{
    setlinestyle(1,0,1);
    setcolor(RED);
    line(SCREEN_WIDTH/2,ycen-TARGET_HEIGHT/2,SCREEN_WIDTH/2,ycen+TARGET_HEIGHT/2);
    line(SCREEN_WIDTH/2-oTARGET_WIDTH/2,ycen,SCREEN_WIDTH/2+oTARGET_WIDTH/2,ycen);
    setlinestyle(0,0,3);
    rectangle((SCREEN_WIDTH-oTARGET_WIDTH)/2, ycen-TARGET_HEIGHT/2,(SCREEN_WIDTH+oTARGET_WIDTH)/2,ycen+TARGET_HEIGHT/2);
}
int main()
{
    int gd = DETECT, gm,i;
    initwindow(SCREEN_WIDTH,SCREEN_HEIGHT,"Poploon");
    s s1[50];      // decalre array of type struct screen
    inf inf1;
    inf1.ssh = inf1.saver1 = 0;
    int GAP;
    setlinestyle(SOLID_LINE,0,1);
    setbkcolor(WHITE);
    for(i=0;i<NO_BU;i++)
    {
        if(i==0)
            s1[i].asize=rd();
        for(s1[i].c=1,s1[i+1].asize=rd(),inf1.indi=i;;)     //i is the identifier of each bubblne here
        {
            int j;
            if(s1[i].c==1)
            {
                if(s1[i].asize == s1[i+1].asize)   // if consecutive bubble are of same size
                {
                    if(s1[i].asize == sr)
                        GAP = rnd(2*sr, 3*sr);       // max gap between consecutive small bubble is 1.5 times min GAP
                    else                            //if(s1.asize[i] == br)
                        GAP = rnd(2*br, 3*br);       // max gap between consecutive big bubble is 1.5 times min GAP
                }
                else
                {
                    GAP = rnd(sr+br, 2*sr+br);      // max limit is approx 1.5 times min vlaue for current value
                }

            }
            if(s1[i].c%GAP==0)      //c for counter
                break;

            bu_gen(s1, &inf1);
            target_box();
            score_keeper(0);            // display current score
            for(j=(inf1.saver1);j<=i;j++)           //all the necessery increments for making all booluns moving
                s1[j].c++;
            if((xcen+s1[inf1.saver1].c)> (500))
                (inf1.saver1)++;
        }
    }
   getch();
   closegraph();
   return 0;
}
