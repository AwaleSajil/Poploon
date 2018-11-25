#include <stdio.h>
#include <stdlib.h>
#include<graphics.h>
#include<conio.h>
#include<dos.h>
#include<time.h>
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define TARGET_WIDTH 1
#define oTARGET_WIDTH 71
#define TARGET_HEIGHT 120
#define ycen 175  //y cordinate center of ellipse
#define xcen -79  // x cordinate center of ellipse
#define sr 65    // radius of smaller balloon
#define br 85   // radius of bigger radius
#define NO_BU 10   /*currently this is the variable that controls how many bubble will be produced*/
#define UP_SCORE 1
struct screen
{
    int c:12;       // c for counters i.e i,j,k,l,m
    int asize:12;   //for storing the size (radius)of bubble in the screen
    unsigned int color:4;       // 0 to 2^4-1  = 15
};
struct addtional_info
{
    int indi;     // indicates how many bullunes are on memory
    int saver1;     // for CPU efficency
    int ssh;        // ssh --> stop scoring high
};
typedef struct screen s;
typedef struct addtional_info inf;

// prototype area
void over(int score,char *reason);
int score_keeper(int change);
int check_w_b(s **sp, inf *inf1);
int rnd(int ll, int hl);
int rd();
void late_process(int *late, int score);
void bu_gen(s **sp,inf *inf1);
void target_box();
s *allocate_the_memory();


void over(int score,char reason[])
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
int check_w_b(s *sp, inf *inf1)
{
    int i;
    static int prev_i;
    for(i=0;i<(inf1->indi);i++)
    {
        int t = (sp[i].asize+(TARGET_WIDTH/2));      //temp -- for i.e radius of circle + screenwidth/2
        int oa = (xcen+sp[i].c) - (SCREEN_WIDTH/2);
        if(oa<=t && oa>=(-t))
        {
            if(prev_i != i)
                inf1->ssh = 0;      //reset the stop score high(ssh) if new bullune is on the target
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
    else if (score>8 && score<21)
        *late = 0;
    else
        *late = rnd(0,5);
}
void bu_gen(s *sp,inf *inf1)
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
    for(i=0;i<(inf1->indi);i++)
    {
        setcolor(WHITE);
        setfillstyle(1,sp[i].color);
        fillellipse(xcen+sp[i].c,ycen,sp[i].asize,sp[i].asize);// bubbles
        setcolor(sp[i].color);
        line(xcen+sp[i].c,ycen+sp[i].asize,xcen+sp[i].c,ycen+sp[i].asize+sr*1.5); //string of bubble
    }
    GetCursorPos(&m_po);
    setfillstyle(1,WHITE);  setcolor(RED);
    fillellipse(m_po.x,m_po.y,3,5);                             // mouse center pointer
    b_t_r = ((m_po.x)>=((SCREEN_WIDTH-oTARGET_WIDTH)/2) && (m_po.x)<=((SCREEN_WIDTH+oTARGET_WIDTH)/2) && (m_po.y)>=(ycen-TARGET_HEIGHT/2) && (m_po.y)<=(ycen+TARGET_HEIGHT/2));
   //  o_o_w = ||
    int t_b = check_w_b(sp,inf1); //check_which_bubble lies in the target area and store it's i value(identifier) of bubble to t_b

    if((GetAsyncKeyState(VK_LBUTTON) || GetAsyncKeyState(VK_RBUTTON))&&((inf1->ssh)++) == 0)
    {
        if (t_b == -1)
        {
            setvisualpage(page);
            cleardevice();
            over(score_keeper(0),"You Clicked Where there is no Ballon");
        }

        if(GetAsyncKeyState(VK_LBUTTON)&& sp[t_b].asize == sr && b_t_r)
            score_keeper(UP_SCORE);
        else if(GetAsyncKeyState(VK_RBUTTON) && sp[t_b].asize == br && b_t_r)
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

s *allocate_the_memory(int items)
{
    s *ptr = (s *)malloc(items*sizeof(s));
    if(ptr == NULL)
    {
        puts("Malloc failed");
        delay(1000);
        exit(1);
    }
    return ptr;
}
int main()
{
    int gd = DETECT, gm,i;
    initwindow(SCREEN_WIDTH,SCREEN_HEIGHT,"Poploon");
    s *sp = allocate_the_memory(NO_BU);       // array to store address  ,+1 is not for terminating/ null char but is for error free in comming lines
    inf inf1;
    inf1.ssh = inf1.saver1 = 0;
    int GAP;
    setlinestyle(SOLID_LINE,0,1);
    setbkcolor(WHITE);
    for(i=0;i<NO_BU;i++)
    {
        if(i==0)
        {
            sp[i].asize=rd();
        }
        for(sp[i].c=1,sp[i+1].asize=rd(),sp[i].color=rnd(0,15),inf1.indi=i+1;;)     //i is the identifier of each bubblne here
        {
            int j;
            //runs only 1 time for one bullune
            if(sp[i].c==1)
            {
                if(sp[i].asize == sp[i+1].asize)   // if consecutive bubble are of same size
                {
                    if(sp[i].asize == sr)
                        GAP = rnd(2*sr, 3*sr);       // max gap between consecutive small bubble is 1.5 times min GAP
                    else                            //if(s1.asize[i] == br)
                        GAP = rnd(2*br, 3*br);       // max gap between consecutive big bubble is 1.5 times min GAP
                }
                else
                {
                    GAP = rnd(sr+br, 2*sr+br);      // max limit is approx 1.5 times min vlaue for current value
                }

            }

            if(sp[i].c%GAP==0)      //c for counter
                break;
            bu_gen(sp, &inf1);
            target_box();
            score_keeper(0);            // display current score
            for(j=0;j<=i;j++)           //all the necessery increments for making all booluns moving
                (sp[j].c)++;
            if((xcen+sp[inf1.saver1].c) > getmaxx())
            {
                inf1.saver1++;
            }

        }

        if(i == (NO_BU-1))
        {
            i =  0;
        }
    }
   getch();
   closegraph();
   return 0;
}

