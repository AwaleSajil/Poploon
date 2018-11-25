#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <conio.h>
#include <dos.h>
#include <time.h>
#include <math.h>
#define TITLE "PoopLoon"
#define SCREEN_WIDTH 800                //1360
#define SCREEN_HEIGHT 600               //700
#define TARGET_HEIGHT 500
#define xcen -79                        // x cordinate center of ellipse at initial point
#define sr 65                           // radius of smaller balloon
#define br 85                           // radius of bigger radius
#define NO_BU 1000                      //currently this is the variable that controls how many bubble will be produced
#define H_S_LOCATION "HighScore.dat"    //high score file data
#define B_DEL 400                       //button delay
#define INF_DEL 300                     //delay for infinte loop i.e. provide resting time for CPU
#define TOP_SC 10                       //Number of high scores to be saved
#define FRAME_DEL 50                    //i.e 20 fps
#define CY_RANGE 5                      //is no of y pixel uncertain between any two frame of the game
#define CX_RANGE 2                      //is no of x pixel uncertain between any two frame of the game
#define RNX_BUBB 5                      //how many no of Rxn time expirement to be performed
typedef struct screen                   //a structure forstoring information of a bubble
{
    int c;                              // c for counters i.e i,j,k,l,m
    int cx;                             //for uncertinity of x
    int cy;                             // for uncertinity of y
    int curr_y;                         //current y position of center of bubble
    int curr_x;                         //current component of x position of center of bubble
    int asize;                          //for storing the size (radius)of bubble in the screen
    unsigned int color:4;               // 0 to 2^4-1  = 15
}s;
typedef struct score_data               //structure for high score dat file
{
    char name[51];                      //name of high score doer
    unsigned int score;
    char date_t[31];                    // date and time of highscore created
}h_s_d;
typedef struct addtional_info           //structure to store the additional information about state of game
{
    int indi;                           // indicates how many bullunes are on memory/have been created
    int saver1;                         // for CPU efficency + (how many balloons have gone out-1)
    int ssh;                            // ssh --> stop scoring high
    int prev_click;                     //for score pani na badhaune out ni nagaraune
}inf;

//essential prototype area
void over(int score,char *reason);
int score_keeper(int change);
int check_w_b(s **sp, inf *inf1);
int rnd(int ll,int hl);
int rd();
int step_process();
int bu_gen(s **sp,inf *inf1);
void target_box();
s *allocate_the_memory();
int welcom();
int c_fptr(FILE *fptr);
int check_h_m_h_s_i_f();

//essential globalvariables
int ycen = SCREEN_HEIGHT/2;
int oTARGET_WIDTH  = SCREEN_WIDTH/3;

void want_ur_name(h_s_d *h)        //this function takes in the h_s_d address + closes graphical window and inputs name
{
    closegraph();
    puts("Enter your name for High Score\n");
    fflush(stdin);
    scanf("%s",(h->name));
    puts("\nThanks, You can press Enter to return to Gaming");
    fflush(stdin);
    getchar();
    initwindow(SCREEN_WIDTH,SCREEN_HEIGHT,TITLE,true,true);
}
int UP_SCORE(s **sp, int t_b)        //this function return the increment score accoring to distance of mouse pointer from center
{
    int d;                          //distance
    int score;
    d = sqrt(pow((sp[t_b]->curr_x +sp[t_b]->c) - (mousex()),2) + pow((sp[t_b]->curr_y) - (mousey()),2));
    if(d == 0)  return ((sp[t_b]->asize));
    score = (sp[t_b]->asize)/(d);
    return score;
}
int D_h_s_sort()                    //returns 0 if sucessfull
{
    int struct_size, i, j;
    h_s_d h1, h2;
    struct_size = sizeof(h1);
    FILE * fptr = fopen(H_S_LOCATION,"rb+");
    if(c_fptr(fptr))   return 1;
    fseek(fptr, 0, SEEK_END);       //go to end of file
    int file_size = ftell(fptr);
    rewind(fptr);                   //go to begining of file
    for (i = 0; i < file_size; i += struct_size)
    {
        for (j = 0; j < file_size - struct_size; j += struct_size)
        {
            fread(&h1, struct_size, 1, fptr);           //rem that poniter itself moved by 1 high score data so no need to move it manually
            fread(&h2, struct_size, 1, fptr);
             if (h1.score < h2.score)
            {
                fseek(fptr, -(struct_size * 2), SEEK_CUR);
                fwrite(&h2, struct_size, 1, fptr);
                fwrite(&h1, struct_size, 1, fptr);
                fseek(fptr, -struct_size, SEEK_CUR);
            }
            else
            {
                fseek(fptr, -struct_size, SEEK_CUR);
            }
        }
        rewind(fptr);
    }
    fclose(fptr);
    return 0;
}

int save_h_s_iff(int score)       //save high score if and only if
{
    h_s_d h;
    time_t the_time = time(NULL);
    FILE *fptr = fopen(H_S_LOCATION,"rb+");
    if(c_fptr(fptr))   return 0;
    h.score = score;
    strcpy(h.date_t,ctime(&the_time));

    if(check_h_m_h_s_i_f() < TOP_SC)
    {
        want_ur_name(&h);
        fseek(fptr,0,SEEK_END);     //to continue writing
        if(fwrite(&h,sizeof(h),1,fptr) < 1)
        {
        perror("Could not save high score " H_S_LOCATION);
        fclose(fptr);
        return 0;
        }
        fclose(fptr);
        return 1;               //this function returns 1 if successfull
    }
    else
    {
        h_s_d h1;
        want_ur_name(&h);
        fseek(fptr,-sizeof(h1),SEEK_END);
        fwrite(&h, sizeof(h1), 1, fptr);
        fclose(fptr);
        return 1;
    }
    fclose(fptr);
    return 0;
}

int is_it_h_s(int score)
{
    FILE *fptr = fopen(H_S_LOCATION,"rb");
    h_s_d h1;
    if(c_fptr(fptr))   return 0;
    if(check_h_m_h_s_i_f() < TOP_SC)
    {
        fclose(fptr);
        return 1;
    }
    D_h_s_sort();
    fseek(fptr,-sizeof(h1),SEEK_END);
    fread(&h1,sizeof(h1),1,fptr);
    if(h1.score < score)
    {
        fclose(fptr);
        return 1;
    }
    fclose(fptr);
    return 0;
}
void free_rem_sp(s **sp, int saver1,int iPlus1)
{
    int i;
    for(i=saver1;i<=iPlus1;i++)
    {
        free(sp[i]);
    }
    free(sp);
}
int c_fptr(FILE *fptr)
{
    if(fptr == NULL)
    {
        perror("Coulnot Open "H_S_LOCATION);
        return 1;
    }
    return 0;
}
void b_but(int x,int y, char str[],int color)
{
    setcolor(color);
    outtextxy(x,y,str);
}
int check_h_m_h_s_i_f()                         //check how many high score data is in the file + it creates the file H_S_LOCATION.dat if absent
{
    int i;
    h_s_d temp;
    FILE *fptr = fopen(H_S_LOCATION,"ab");      // create the file H_S_Location.dat if absent
        if(c_fptr(fptr))   exit(1);             //checks fptr
    fclose(fptr);                               // if there file, append it then again open it after closing
    fptr = fopen(H_S_LOCATION,"rb");
    if(c_fptr(fptr))   exit(1);
    for(i=0;fread(&temp,sizeof(temp),1,fptr) == 1;i++);    // do nothing loop just counts the score + fread return the number of data red successfully + second opton of this code is to use fseek and ftell divid by sizeof temp    fclose(fptr);
    fclose(fptr);
    return i;                                   // no of high scores is returned
}
void over(int score,char reason[])
{
    char g_over[] = "GAME OVER !!!";
    char sc[15];
    int bkcolor = 0; //black
    int localbol = is_it_h_s(score);
    if(localbol)    bkcolor = 4; //red
    setbkcolor(bkcolor);
    settextstyle(3,0,8);
    outtextxy((getmaxx()/2)-(textwidth(g_over)/2),(getmaxy()/2)-(textheight(g_over)/2),g_over);
    sprintf(sc,"Score is %d",score);
    settextstyle(3,0,2);
    outtextxy((getmaxx()/2)-(textwidth(reason)/2),(getmaxy()/2)-3*(textheight(reason)),reason);
    settextstyle(3,0,5);
    outtextxy((getmaxx()/2)-(textwidth(sc)/2),(getmaxy()/2)+(textheight(sc)/1.5),sc);
    delay(3000);
    if(localbol)
        save_h_s_iff(score);
    return;
}
int score_keeper(int change)
{
    static int sc_n=0;       //score in numbers
    char score[16];
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
        int t = sp[i]->asize;      //max distance
        int d = sqrt(pow((sp[i]->curr_x +sp[i]->c) - (mousex()),2) + pow((sp[i]->curr_y) - (mousey()),2));    //distance between pointer and bubble center
            if(d<=t)
            {
                if(prev_i != i)
                    inf1->ssh = 0;      //reset the stop score high(ssh) if new bullune is on the target
                if(sp[i]->color != 15)
                    return (prev_i=i);
                if(sp[i]->color == 15 && inf1->prev_click == 1)
                    return (prev_i=i);
            }
        }

    return -1;          // i.e no bullunes lie at that position
}
long ud_rnd()
{
    static long a=123456879, b=356789879, c=578762099;
    long t;
    a ^= a << 13;
    a ^= a >> 17;
    a ^= a << 5;
    t = a;
    a = b;
    b = c;
    c = t ^ a ^ b;
    return c;
}
int rnd(int ll,int hl) // for generating random numbers from lower limit ll, to higher limit hl
{
    long raw_t = ((int)time(NULL) + abs(ud_rnd()));
    srand(raw_t);
    return (rand()%(hl-ll+1) + ll);
}
int rd()
{
    if(rnd(0,1))    return br;
    else            return sr;
}
int step_process()
{
    int late, score = score_keeper(0);
    late = (12*score)/(sr) + 7;
    if(score >100 && score < 150)    late = rnd(25,35);
    if(score > 149)     late = rnd(30,50);
    return late;
}
void y_uncer_val(s **sp,int i)
{
    int dy = rnd(-2*CY_RANGE,2*CY_RANGE); //randomize y cordinates;
    if((sp[i]->curr_y + dy) > ((SCREEN_HEIGHT+TARGET_HEIGHT)/2 - sp[i]->asize))         // over do cases
        dy = -rnd(0,CY_RANGE);
    if((sp[i]->curr_y + dy) < ((SCREEN_HEIGHT-TARGET_HEIGHT)/2 + sp[i]->asize))         // overdo cases
        dy = rnd(0,CY_RANGE);
    sp[i]->cy = dy;
}
void x_uncer_val(s **sp,int i)
{
    int dx;
    dx = rnd(-2*CX_RANGE,2*CX_RANGE);
    if(i==0)
    {
        sp[i]->cx = rnd(0,2*CX_RANGE);
        return;
    }
    if(((sp[i-1]->curr_x+sp[i-1]->c)-(sp[i]->curr_x +sp[i]->c+ dx))   <   (sp[i]->asize + sp[i-1]->asize))  dx = -rnd(0,CX_RANGE);
    if(((sp[i]->curr_x +sp[i]->c+ dx)-(sp[i+1]->curr_x+sp[i+1]->c))   <   (sp[i]->asize + sp[i+1]->asize))  dx = rnd(0,CX_RANGE);   //over do cases
    sp[i]->cx = dx;
}
int bu_gen(s **sp,inf *inf1)
{
    int i;
    int b_t_r;        // is boolen variable for knowing if the mouse is witnin the target box
    static int page=0;
    setactivepage(page);
    setvisualpage(1-page);
    cleardevice();
    setlinestyle(0,0,2);
    if(!(GetAsyncKeyState(VK_LBUTTON) || GetAsyncKeyState(VK_RBUTTON)))
        inf1->prev_click = 0;
    for(i=(inf1->saver1);i<(inf1->indi);i++)
    {
        setcolor(WHITE);
        setfillstyle(1,sp[i]->color);
        sp[i]->curr_y += (sp[i]->cy);           //set current y position
        sp[i]->curr_x += (sp[i]->cx);           //set current x position
        fillellipse(sp[i]->curr_x +sp[i]->c,sp[i]->curr_y,sp[i]->asize,sp[i]->asize);// bubbles
        y_uncer_val(sp,i); //randomize y cordinates
        x_uncer_val(sp,i);    //randomize x cordinates
        setcolor(sp[i]->color);
    }
    setfillstyle(1,WHITE);  setcolor(RED);
    fillellipse(mousex(),mousey(),3,3);                             // mouse center pointer
    b_t_r = ((mousex())>=((SCREEN_WIDTH-oTARGET_WIDTH)/2) && (mousex())<=((SCREEN_WIDTH+oTARGET_WIDTH)/2) && (mousey())>=(ycen-TARGET_HEIGHT/2) && (mousey())<=(ycen+TARGET_HEIGHT/2));
    int t_b = check_w_b(sp,inf1); //check_which_bubble lies in the target area and store it's i value(identifier) of bubble to t_b

    if((GetAsyncKeyState(VK_LBUTTON) || GetAsyncKeyState(VK_RBUTTON)) && t_b == -1 && inf1->prev_click == 0)
    {
            setvisualpage(page);
            cleardevice();
            over(score_keeper(0),"No Balloon there");
            return 0;
    }
    if((GetAsyncKeyState(VK_LBUTTON) || GetAsyncKeyState(VK_RBUTTON))&&((inf1->ssh)++) == 0)
    {
        if(GetAsyncKeyState(VK_LBUTTON)&& sp[t_b]->asize == sr && b_t_r && inf1->prev_click==0)
        {
            setcolor(sp[t_b]->color = 15);      //disappear the bubble(white)
            score_keeper(UP_SCORE(sp,t_b));
            inf1->prev_click = 1;
        }

        else if(GetAsyncKeyState(VK_RBUTTON) && sp[t_b]->asize == br && b_t_r && inf1->prev_click==0)
        {
            setcolor(sp[t_b]->color = 15);      //disappear the bubble
            score_keeper(UP_SCORE(sp,t_b));
            inf1->prev_click = 1;
        }
        else if(!b_t_r)
        {
            setvisualpage(page);
            cleardevice();
            over(score_keeper(0),"Your cursor is outside target box");
            return 0;
        }

        else if(inf1->prev_click==0)
            {
                setvisualpage(page);
                cleardevice();
                over(score_keeper(0),"Wrong Click for ballon");
                return 0;
            }
    }
    page = 1-page;
    delay(FRAME_DEL);
    return 1;
}
void target_box()
{
    int score = score_keeper(0);
    if(oTARGET_WIDTH > 199)  oTARGET_WIDTH = SCREEN_WIDTH/3 - score;
    if(oTARGET_WIDTH < 200)  oTARGET_WIDTH = rnd(150,200);
    setcolor(RED);
    setlinestyle(0,0,3);
    rectangle((SCREEN_WIDTH-oTARGET_WIDTH)/2, ycen-TARGET_HEIGHT/2,(SCREEN_WIDTH+oTARGET_WIDTH)/2,ycen+TARGET_HEIGHT/2);
    setlinestyle(1,0,1);
    line(SCREEN_WIDTH/2,ycen-TARGET_HEIGHT/2,SCREEN_WIDTH/2,ycen-TARGET_HEIGHT/2+TARGET_HEIGHT/16);      //vertical line obserable
    line(SCREEN_WIDTH/2,ycen+TARGET_HEIGHT/2,SCREEN_WIDTH/2,ycen+TARGET_HEIGHT/2-TARGET_HEIGHT/16);      //vertical line obserable
    line(SCREEN_WIDTH/2-oTARGET_WIDTH/2,ycen,SCREEN_WIDTH/2-oTARGET_WIDTH/2+oTARGET_WIDTH/16,ycen);      //horizontal line
    line(SCREEN_WIDTH/2+oTARGET_WIDTH/2,ycen,SCREEN_WIDTH/2+oTARGET_WIDTH/2-oTARGET_WIDTH/16,ycen);      //horizontal line
}

s *allocate_the_memory()
{
    s *ptr = (s *)malloc(sizeof(s));
    if(ptr == NULL)
    {
        puts("Malloc failed");
        exit(1);
    }
    return ptr;
}
void new_game()
{
    initwindow(SCREEN_WIDTH,SCREEN_HEIGHT,TITLE,true,true);
    s *sp[NO_BU+1];       // array to store address  ,+1 is not for terminating/ null char but is for error free in comming lines
    inf inf1;
    inf1.ssh = inf1.saver1 = inf1.prev_click= 0;
    int i,GAP;
    setbkcolor(WHITE);
    for(i=0;i<NO_BU;i++)
    {
        if(i==0)
        {
            sp[i] = allocate_the_memory();
            sp[i]->asize=rd();                  //rd return radius of big or small bullune
            sp[i]->curr_x=xcen;
            sp[i]->c=0;
        }
        sp[i+1] = allocate_the_memory();  //No_BU +1 balloon ko lagi ko size allocation
        for(sp[i+1]->c=0,sp[i+1]->asize=rd(),sp[i]->color=rnd(0,14),inf1.indi=i+1,sp[i]->cy = 0,sp[i]->cx = 0,sp[i]->curr_y=ycen,sp[i+1]->curr_x=xcen;;)     //i is the identifier of each bubblne here
        {
            int j;
            //runs only 1 time for one bullune
            if(sp[i]->c==0)
            {
                if(sp[i]->asize == sp[i+1]->asize)   // if consecutive bubble are of same size
                {
                    if(sp[i]->asize == sr)
                        GAP = rnd(2*sr, 3*sr);       // max gap between consecutive small bubble is 1.5 times min GAP
                    else                            //if(s1.asize[i] == br)
                        GAP = rnd(2*br, 3*br);       // max gap between consecutive big bubble is 1.5 times min GAP
                }
                else
                    GAP = rnd(sr+br, 2*sr+br);      // max limit is approx 1.5 times min vlaue for current value
            }
            if((sp[i]->c)>(GAP))      //c for counter  //do not generqate until minimum gap size is there ...baloon is created when c for each balloon increases till gap size
                break;
            if(bu_gen(sp, &inf1) != 1)
            {
                free_rem_sp(sp,inf1.saver1,i+1);        //free all the memory remainig to free up
                score_keeper(-score_keeper(0));         //reset the score to 0 for new game
                oTARGET_WIDTH = SCREEN_WIDTH/3;         //reset the obserbable target width backto initial
                return;                                 // goback tocalling function i.e main
            }
            target_box();                               //display target box
            score_keeper(0);                            // display current score...vale of score remaining the same
            for(j=(inf1.saver1);j<(inf1.indi);j++)      //all the necessery increments for making all booluns moving //inf1.saver1=0 initially
            {
                (sp[j]->c)+= step_process();            //sp[j]->c controls natural speed i,e accelearatingfrom left to right
            }
            if((xcen+sp[inf1.saver1]->c) > getmaxx()+br)    //when balloon goes out of screen and that distance is equalt to br, then free the space and saver value  is increased
            {
                free(sp[(inf1.saver1)]);
                inf1.saver1++;
            }
        }
    }
   getch();
   closegraph();
}
int ycalc(int nt,int n,int font=8,int txt_size=5)
{
    int total_heit,y,ygap=10,txt_heit;
    settextstyle(font,0,txt_size);
    txt_heit = textheight("A");
    total_heit = nt*txt_heit + (nt-1)*ygap;
    y = (SCREEN_HEIGHT/2 - total_heit/2) +(n-1)*(txt_heit + ygap);
    return y;
}
int button(int y, char *but_nam)
{
    int txt_color , b_bool; //black by default
    settextstyle(8,0,5);
    b_bool = mousex() > (SCREEN_WIDTH/2 - textwidth(but_nam)/2) && mousex() < (SCREEN_WIDTH/2 + textwidth(but_nam)/2) && (mousey()) > (y) && (mousey()) < (y+textheight(but_nam));
    txt_color = b_bool ? 9:0;   //green
    b_but(SCREEN_WIDTH/2 - textwidth(but_nam)/2, y, but_nam, txt_color);

    if(b_bool && GetAsyncKeyState(VK_LBUTTON))
    {
        b_but(SCREEN_WIDTH/2 - textwidth(but_nam)/2, y, but_nam, 12);   //red
        delay(B_DEL);
        if(!GetAsyncKeyState(VK_LBUTTON))       //extra protection
        return 1;  //1 for successfulclick                             //not click or not on text
    }
    return 0;//o for unsuccessfulclick
}
int welcom()
{
    initwindow(SCREEN_WIDTH,SCREEN_HEIGHT,TITLE,true,true);
    char title[] = TITLE;
    char we[][21] = {"Sajil Awale","Shrey Niraula","Pratik Luitel","Pujan Budhathoki"};
    int sep;
    int i, sum=0;
    int nt = 4;     //no of buttons
        // names
        {
            setbkcolor(WHITE);// for text backgound color
            settextstyle(8,0,1);
            sep = (SCREEN_WIDTH - textwidth(we[0]) -textwidth(we[1]) -textwidth(we[2]) -textwidth(we[3]))/5;    // (screenwidth-total name length)/5
            for(i=0;i<4;i++)                        //access string array by loop
            {
                b_but(sum+sep,SCREEN_HEIGHT-1.5*textheight(we[0]),we[i],i+2);       //x=sum+sep, y=SCREEN_HEIGHT-2*textheight(we[0]), i+2=color
                sum +=(sep+textwidth(we[i]));                                       //sum allocates space eqaul to sep+name length
            }
        }
        floodfill(SCREEN_WIDTH-1,SCREEN_HEIGHT-1,WHITE);                            // for white background
    // for title
        {
            settextstyle(3,0,7);
            b_but(SCREEN_WIDTH/2-textwidth(title)/2,SCREEN_HEIGHT/10,title,rnd(0,14));
        }
    for(char ne_gam[] = "New Game",high_sc[] = "High Score",close[] = "Exit",inst[] = "Instructions",mini_game[] = "Rxn Time";;)
    {
        //buttons
        if(button(ycalc(nt,1),ne_gam))      return 0;                        //New game button
        if(button(ycalc(nt,4),high_sc))     return 1;                       //high score button
        if(button(SCREEN_HEIGHT-2*textheight(close),close))     return -1; // exit button
        if(button(ycalc(nt,2),mini_game))     return 2;                   //rxn game
        if(button(ycalc(nt,3),inst))     return 3;                       //instruction button
        delay(INF_DEL);
    }
}
int print_h_s()
{
    initwindow(SCREEN_WIDTH,SCREEN_HEIGHT,TITLE,true,true);
    FILE *fptr;
    h_s_d h1;
    int i;
    char h_s_s[]= "High Scores";
    fptr = fopen(H_S_LOCATION,"rb");
    cleardevice();
    setcolor(5);
    floodfill(SCREEN_WIDTH-1,SCREEN_HEIGHT-1,WHITE);    // for white background
    setbkcolor(WHITE);
    if(c_fptr(fptr))    return 0;
    if(check_h_m_h_s_i_f() == 0)
    {
        char nofile[] = "Looks like it's you first time";
        b_but(SCREEN_WIDTH/2-textwidth(nofile)/2,SCREEN_HEIGHT/2-textheight(nofile)/2,nofile,0);
    }
    // title i.e High score
    if(D_h_s_sort())    puts("High score couldnot be sort");              //sort highscore in descending order
    settextstyle(8,0,5);
    b_but(SCREEN_WIDTH/2-textwidth(h_s_s)/2,textheight(h_s_s)/2,h_s_s,0);
    settextstyle(8,0,3);
    for(i=0;fread(&h1,sizeof(h1),1,fptr) == 1; i++)     //fread returns 1 if they were sucessful
    {
        char buffer[300];
        sprintf(buffer,"%2d: %-7s : %d :On %s",i+1,h1.name,h1.score,h1.date_t);
        b_but(SCREEN_WIDTH/2-textwidth(buffer)/2,2*textheight(h_s_s)+(i+1)*textheight(buffer),buffer,0);
    }
    fclose(fptr);
    for(char g_b_w_s[] = "Go Back";;)
    {
        if(button(SCREEN_HEIGHT-1*textheight(g_b_w_s),g_b_w_s))   return 1;  //go back to welcome screen
        delay(INF_DEL);
    }

    return 0;
}
void rxn_game()
{
    char mess[21] = "OUT OUT !!!";
    char ins[] = "Click bullble as soon as bubble gets filled";
    int sum= 0,fill_color;
    initwindow(SCREEN_WIDTH,SCREEN_HEIGHT,TITLE,true,true);
    floodfill(SCREEN_WIDTH-1,SCREEN_HEIGHT-1,WHITE);
    setbkcolor(WHITE);  settextstyle(8,0,3);
    b_but(SCREEN_WIDTH/2-textwidth(ins)/2,SCREEN_HEIGHT/5,ins,0);
    for(int j=0;j<RNX_BUBB;j++)
    {
        int i;
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
            return;
        }
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
    return;
}
int instruction()
{
    char inst[] = "Instructions", line[][61] = {"Place mouse pointer inside Targetbox", "Click Left Click for Small Bubble","Click Right Click for Big Bubble","Click it as close to the center of bubble", "Game is Over when you misclick it", "There is Rxn mini game for practicing Reflex"};
    int i;
    initwindow(SCREEN_WIDTH,SCREEN_HEIGHT,TITLE,true,true);
    setbkcolor(WHITE);
    floodfill(SCREEN_WIDTH-1,SCREEN_HEIGHT-1,WHITE);    // for white background
    settextstyle(8,0,5);
    b_but(SCREEN_WIDTH/2-textwidth(inst)/2,textheight(inst)/2,inst,0);
    settextstyle(8,0,3);
    for(i=0;i<6;i++)
    {
        char buffer[71];
        sprintf(buffer,"%2d: %-45s",i+1,line[i]);
        b_but(SCREEN_WIDTH/2-textwidth(buffer)/2,2*textheight(buffer)+(i+2)*textheight(buffer),buffer,0);
    }
    for(char g_b_w_s[] = "Go Back";;)
    {
        if(button(SCREEN_HEIGHT-1*textheight(g_b_w_s),g_b_w_s))   return 1;  //go back to welcome screen
        delay(INF_DEL);
    }
    return 0;
    getch();
}
int main()
{
    int gd = DETECT, gm;
    check_h_m_h_s_i_f();        // to create file if not created
    for(;;)
    {
        int r = welcom();       // r is the returned value of mouse position by function welcome
        closegraph();
        switch(r)
        {
            case 0: new_game(); closegraph();   break;
            case 1: if(print_h_s()==1)
                    {
                        closegraph();
                        continue;
                    }   break;
            case 2: rxn_game(); closegraph();   break;
            case 3: if(instruction()==1)
                    {
                        closegraph();           // closegraph not done in function
                        continue;
                    }   break;
            case -1: closegraph();  exit(0);    //when exit is clicked on welcome screen
        }
    }
    closegraph();
    return 0;
}


