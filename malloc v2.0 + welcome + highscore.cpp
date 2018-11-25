#include <stdio.h>
#include <stdlib.h>
#include<graphics.h>
#include<conio.h>
#include<dos.h>
#include<time.h>
#define TITLE "Pooploon"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define TARGET_WIDTH 1
#define oTARGET_WIDTH 71
#define TARGET_HEIGHT 120
#define ycen 175  //y cordinate center of ellipse
#define xcen -79  // x cordinate center of ellipse
#define sr 65    // radius of smaller balloon
#define br 85   // radius of bigger radius
#define NO_BU 100   /*currently this is the variable that controls how many bubble will be produced*/
#define UP_SCORE 1
#define H_S_LOCATION "HighScore.dat"
typedef struct screen
{
    int c:12;       // c for counters i.e i,j,k,l,m
    int asize:12;   //for storing the size (radius)of bubble in the screen
    unsigned int color:4;       // 0 to 2^4-1  = 15
}s;
typedef struct addtional_info
{
    int indi;     // indicates how many bullunes are on memory/have been created
    int saver1;     // for CPU efficency
    int ssh;        // ssh --> stop scoring high
}inf;
typedef struct score_data
{
    char name[51];      //not sure how to input string in graphics mode
    unsigned int score;
    char date_t[31];
}h_s_d;

// prototype area
void over(int score,char *reason);
int score_keeper(int change);
int check_w_b(s **sp, inf *inf1);
int rnd(int ll, int hl);
int rd();
void late_process(int *late, int score);
int bu_gen(s **sp,inf *inf1);
void target_box();
s *allocate_the_memory();
int welcom();


int c_fptr(FILE *fptr)
{
    if(fptr == NULL)
    {
        perror("Coulnot Open "H_S_LOCATION);
        return 1;
    }
    return 0;
}
void b_but(int x, int y, char str[], int color)
{
    setcolor(color);
    outtextxy(x,y,str);
}
int check_h_m_h_s_i_f()     //check how many high score data is in the file + it creates the file H_S_LOCATION.dat if absent
{
    int i;
    h_s_d temp;
    FILE *fptr = fopen(H_S_LOCATION,"ab");      // create the file H_S_Location.dat if absent
        fclose(fptr);
    if(c_fptr(fptr))   exit(1);
    fptr = fopen(H_S_LOCATION,"rb");
    if(fptr == NULL)
    {
        perror("Coulnot Open "H_S_LOCATION);
        exit(1);
    }
    for(i=0;fread(&temp,sizeof(temp),1,fptr) == 1;i++);
    fclose(fptr);
    return i;
}
int save_h_s(int score)
{
    h_s_d hh;
    int i;
    time_t the_time = time(NULL);
    FILE *fptr = fopen(H_S_LOCATION,"ab");
    hh.score = score;
    strcpy(hh.date_t,ctime(&the_time));
    if(fptr == NULL)
    {
        perror("Could not open " H_S_LOCATION);
        return 0;
    }
    if(fwrite(&hh,sizeof(hh),1,fptr) < 1)
    {
        perror("Could not save high score " H_S_LOCATION);
        fclose(fptr);
        return 0;
    }
    fclose(fptr);
    return 1;
}
int delete_data(int h2score)
{
    FILE *fptr;
    FILE *temp_ptr;
    h_s_d myrecord;
    fptr = fopen(H_S_LOCATION,"rb");
    if(fptr == NULL)
    {
        perror("Could not open "H_S_LOCATION);
        return 0;
    }
    temp_ptr = fopen("temp.dat","wb");
    if(temp_ptr == NULL)
    {
        perror("Could not open "H_S_LOCATION);
        return 0;
    }
    while(fread(&myrecord,sizeof(myrecord),1,fptr) == 1)
    {
        if(h2score != myrecord.score)
        {
            fwrite(&myrecord, sizeof(myrecord),1,temp_ptr);
        }
    }
    fclose(fptr);
	fclose(temp_ptr);

	printf("%d \n",remove(H_S_LOCATION));           //returning -1
	printf("%d",rename("temp.dat", H_S_LOCATION));  // returning -1
	return 1;
}
int is_it_h_s(int score)
{
    FILE *fptr = fopen(H_S_LOCATION,"rb");
    h_s_d h2;
    int boln=1;
    int i;
    if(fptr==NULL)
    {
        perror("Could not open "H_S_LOCATION);
        return 0;
    }
    for(i=0;fread(&h2,sizeof(h2),1,fptr)==1;i++)
    {
        if(h2.score < score)
        {
            fclose(fptr);
            if(delete_data(h2.score) == 0)
                printf("Could not delete old high score\n");
            return 1;
        }
    }
    fclose(fptr);
    return 0;
}
void over(int score,char reason[])
{
    char g_over[] = "GAME OVER !!!";
    char sc[15];
    int bkcolor = 0; //black
    if(check_h_m_h_s_i_f() < 11)
    {
        if(save_h_s(score))
            bkcolor = 4; //red
    }

    if(is_it_h_s(score))        // see if it is on top ten(say) high score
    {
        if(save_h_s(score))
            bkcolor = 4; //red
    }
    setbkcolor(bkcolor);
    settextstyle(3,0,8);
    outtextxy((getmaxx()/2)-(textwidth(g_over)/2),(getmaxy()/2)-(textheight(g_over)/2),g_over);
    sprintf(sc,"Score is %d",score);
    settextstyle(3,0,2);
    outtextxy((getmaxx()/2)-(textwidth(reason)/2),(getmaxy()/2)-3*(textheight(reason)),reason);
    settextstyle(3,0,5);
    outtextxy((getmaxx()/2)-(textwidth(sc)/2),(getmaxy()/2)+(textheight(sc)/1.5),sc);
    delay(5000);
    return;
}
int score_keeper(int change)
{
    static int sc_n=0;       //score in numbers
    char score[101];
    sc_n += change;
    sprintf(score,"Score: %d",sc_n);
    settextstyle(3,0,3);
    b_but(getmaxx()-textwidth(score),textheight(score),score,BLACK);
    setcolor(WHITE);
    return sc_n;
}
int check_w_b(s **sp, inf *inf1)
{
    int i;
    static int prev_i;
    for(i=(inf1->saver1);i<(inf1->indi);i++)
    {
        int t = (sp[i]->asize+(TARGET_WIDTH/2));      //temp -- for i.e radius of circle + screenwidth/2
        int oa = (xcen+sp[i]->c) - (SCREEN_WIDTH/2);
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
    else if(score>2 && score<10)
        *late = 3;
    else if (score>9 && score<21)
        *late = 2;
    else if (score>20 && score<31)
        *late = 1;
    else if (score>30 && score<41)
        *late = 0;
    else
        *late = rnd(0,5);
}
int bu_gen(s **sp,inf *inf1)
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
    for(i=(inf1->saver1);i<(inf1->indi);i++)
    {
        setcolor(WHITE);
        setfillstyle(1,sp[i]->color);
        fillellipse(xcen+sp[i]->c,ycen,sp[i]->asize,sp[i]->asize);// bubbles
        setcolor(sp[i]->color);
        line(xcen+sp[i]->c,ycen+sp[i]->asize,xcen+sp[i]->c,ycen+sp[i]->asize+sr*1.5); //string of bubble
    }
    GetCursorPos(&m_po);
    setfillstyle(1,WHITE);  setcolor(RED);
    fillellipse(m_po.x,m_po.y,3,5);                             // mouse center pointer
    b_t_r = ((m_po.x)>=((SCREEN_WIDTH-oTARGET_WIDTH)/2) && (m_po.x)<=((SCREEN_WIDTH+oTARGET_WIDTH)/2) && (m_po.y)>=(ycen-TARGET_HEIGHT/2) && (m_po.y)<=(ycen+TARGET_HEIGHT/2));
    int t_b = check_w_b(sp,inf1); //check_which_bubble lies in the target area and store it's i value(identifier) of bubble to t_b

    if((GetAsyncKeyState(VK_LBUTTON) || GetAsyncKeyState(VK_RBUTTON))&&((inf1->ssh)++) == 0)
    {
        if (t_b == -1)
        {
            setvisualpage(page);
            cleardevice();
            over(score_keeper(0),"You Clicked Where there is no Ballon");
            return 0;
        }

        if(GetAsyncKeyState(VK_LBUTTON)&& sp[t_b]->asize == sr && b_t_r)
            score_keeper(UP_SCORE);
        else if(GetAsyncKeyState(VK_RBUTTON) && sp[t_b]->asize == br && b_t_r)
            score_keeper(UP_SCORE);
        else
            {
                setvisualpage(page);
                cleardevice();
                over(score_keeper(0),"Wrong Click");
                return 0;
            }
    }
    page = 1-page;
    late_process(&late,score_keeper(0));
    delay(late);
    return 1;
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

s *allocate_the_memory()
{
    s *ptr = (s *)malloc(28);
    if(ptr == NULL)
    {
        puts("Malloc failed");
        delay(1000);
        exit(1);
    }
    return ptr;
}
void new_game()
{
    s *sp[NO_BU+1];       // array to store address  ,+1 is not for terminating/ null char but is for error free in comming lines
    inf inf1;
    inf1.ssh = inf1.saver1 = 0;
    int GAP,i;
    setlinestyle(SOLID_LINE,0,1);
    setbkcolor(WHITE);
    for(i=0;i<NO_BU;i++)
    {
        if(i==0)
        {
            sp[i] = allocate_the_memory();
            sp[i]->asize=rd();
        }
        sp[i+1] = allocate_the_memory();
        for(sp[i]->c=1,sp[i+1]->asize=rd(),sp[i]->color=rnd(0,15),inf1.indi=i+1;;)     //i is the identifier of each bubblne here
        {
            int j;
            //runs only 1 time for one bullune
            if(sp[i]->c==1)
            {
                if(sp[i]->asize == sp[i+1]->asize)   // if consecutive bubble are of same size
                {
                    if(sp[i]->asize == sr)
                        GAP = rnd(2*sr, 3*sr);       // max gap between consecutive small bubble is 1.5 times min GAP
                    else                            //if(s1.asize[i] == br)
                        GAP = rnd(2*br, 3*br);       // max gap between consecutive big bubble is 1.5 times min GAP
                }
                else
                {
                    GAP = rnd(sr+br, 2*sr+br);      // max limit is approx 1.5 times min vlaue for current value
                }

            }

            if(sp[i]->c%GAP==0)      //c for counter
                break;
            if(bu_gen(sp, &inf1) != 1)
            {
                return;
            }
            target_box();
            score_keeper(0);            // display current score
            for(j=(inf1.saver1);j<=i;j++)           //all the necessery increments for making all booluns moving
                (sp[j]->c)++;
            if((xcen+sp[inf1.saver1]->c) > getmaxx())
            {
                free(sp[(inf1.saver1)]);
                inf1.saver1++;
            }

        }
    }
   getch();
   closegraph();
}
int welcom()
{
    POINT m_po;        // declare a variable for mouse position
    char ne_gam[] = "New Game";
    char high_sc[] = "High Score";
    char title[] = TITLE;
    char we[][21] = {"Sajil Awale","Shrey Niraula","Pratik Luitel","Pujan Budhathoki"};
    int sep;
    int i, sum=0,new_txt_color=0,high_txt_color=0;
    int n_g_bo,h_s_bo;

        cleardevice();
        // names
        setbkcolor(WHITE);
        settextstyle(1,0,1);
        sep = (SCREEN_WIDTH - textwidth(we[0]) -textwidth(we[1]) -textwidth(we[2]) -textwidth(we[3]))/5;
        for(i=0;i<4;i++)
        {
            b_but((i+1)*sep+sum,SCREEN_HEIGHT-2*textheight(we[0]),we[i],i+2);
            sum += textwidth(we[i]);
        }
        setcolor(5);
        floodfill(SCREEN_WIDTH-1,SCREEN_HEIGHT-1,WHITE);    // for white background
// for title
        settextstyle(3,0,7);
        b_but(SCREEN_WIDTH/2-textwidth(title)/2,SCREEN_HEIGHT/10,title,5);
    for(;;)
    {
        //options
        settextstyle(8,0,5);
        b_but(SCREEN_WIDTH/2-textwidth(ne_gam)/2,SCREEN_HEIGHT/2.1-textheight(ne_gam),ne_gam,new_txt_color);
        line(SCREEN_WIDTH/2-textwidth(ne_gam)/2,SCREEN_HEIGHT/2.1-textheight(ne_gam),SCREEN_WIDTH/2+textwidth(ne_gam)/2,SCREEN_HEIGHT/2.1-textheight(ne_gam));
        b_but(SCREEN_WIDTH/2-textwidth(high_sc)/2,SCREEN_HEIGHT/2,high_sc,high_txt_color);
        line(SCREEN_WIDTH/2-textwidth(high_sc)/2,SCREEN_HEIGHT/2+textheight(high_sc),SCREEN_WIDTH/2+textwidth(high_sc)/2,SCREEN_HEIGHT/2+textheight(high_sc));


        GetCursorPos(&m_po);
        n_g_bo = (m_po.x)> (SCREEN_WIDTH/2-textwidth(ne_gam)/2) && (m_po.x) < (SCREEN_WIDTH/2+textwidth(ne_gam)/2) && (m_po.y-15) > (SCREEN_HEIGHT/2.1-textheight(ne_gam)) && (m_po.y-15) < (SCREEN_HEIGHT/2.1);
        h_s_bo = (m_po.x) > (SCREEN_WIDTH/2-textwidth(high_sc)/2) && (m_po.x) < (SCREEN_WIDTH/2+textwidth(high_sc)/2) && (m_po.y-30) > (SCREEN_HEIGHT/2) && (m_po.y-30) < (SCREEN_HEIGHT/2+textheight(high_sc));

        new_txt_color = n_g_bo ? 2:0;
        high_txt_color = h_s_bo ? 2:0;

        if(n_g_bo && GetAsyncKeyState(VK_LBUTTON))
        {
            settextstyle(8,0,5);
            b_but(SCREEN_WIDTH/2-textwidth(ne_gam)/2,SCREEN_HEIGHT/2.1-textheight(ne_gam),ne_gam,11);
            delay(500);
            return 0;
        }

        if(h_s_bo && GetAsyncKeyState(VK_LBUTTON))
        {
            settextstyle(8,0,5);
            b_but(SCREEN_WIDTH/2-textwidth(high_sc)/2,SCREEN_HEIGHT/2,high_sc,11);
            delay(200);
            cleardevice();
            return 1;
        }
        delay(50);
    }
    getch();
}
int print_h_s()
{
    POINT m_po;
    FILE *fptr;
    h_s_d h1;
    int i;
    char h_s_s[]= "High Scores";
    char g_b_w_s[] = "Go Back";
    fptr = fopen(H_S_LOCATION,"rb");
    if(fptr == NULL)
    {
        char nofile[] = "Looks like it's you first time";
        perror("Could not open "H_S_LOCATION);
        b_but(SCREEN_WIDTH/2-textwidth(nofile)/2,SCREEN_HEIGHT/2-textheight(nofile)/2,nofile,0);
        getch();
        exit(1);
    }
    // title i.e High score
    settextstyle(8,0,5);
    b_but(SCREEN_WIDTH/2-textwidth(h_s_s)/2,textheight(h_s_s)/2,h_s_s,0);
    settextstyle(8,0,3);
    for(i=0;fread(&h1,sizeof(h1),1,fptr) == 1; i++)     //fread returns 1 if they were sucessful
    {
        char buffer[300];
        sprintf(buffer,"Score:  %d  On %s",h1.score,h1.date_t);
        b_but(SCREEN_WIDTH/2-textwidth(buffer)/2,1.5*textheight(h_s_s)+(i+1)*textheight(buffer),buffer,0);
    }
    fclose(fptr);

    settextstyle(8,0,5);
    for(;;)
    {
        int boln=0;
        int new_txt_color = 0;
        //go back to welcome screen
        b_but(SCREEN_WIDTH/2-textwidth(g_b_w_s)/2,SCREEN_HEIGHT-1.5*textheight(g_b_w_s),g_b_w_s,new_txt_color);

        GetCursorPos(&m_po);
        boln = (m_po.x) > (SCREEN_WIDTH/2-textwidth(g_b_w_s)/2) && (m_po.x) < (SCREEN_WIDTH/2+textwidth(g_b_w_s)/2) && (m_po.y) > (SCREEN_HEIGHT-1.5*textheight(g_b_w_s)) && (m_po.y) > (SCREEN_HEIGHT-0.5*textheight(g_b_w_s));
        new_txt_color = boln ? 2:0;

        if(boln && GetAsyncKeyState(VK_LBUTTON))
        {
            b_but(SCREEN_WIDTH/2-textwidth(g_b_w_s)/2,SCREEN_HEIGHT-1.5*textheight(g_b_w_s),g_b_w_s,11);
            delay(100);
            return 1;       // go to welcom screen
        }

    }
    return 0;

}
int main()
{
    int gd = DETECT, gm;
    initwindow(SCREEN_WIDTH,SCREEN_HEIGHT,TITLE);
    for(;;)
    {
        if(welcom() == 0)
        {
            new_game();
        }
        else
        {
            if(print_h_s()==1)
                continue;
            getch();
        }
    }
    return 0;
}

