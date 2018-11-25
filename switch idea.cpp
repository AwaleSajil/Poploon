#include <stdio.h>
#include <stdlib.h>
#include<graphics.h>
#include<conio.h>
#include<dos.h>
#include<time.h>
#define GAP 180   // gap between consecutive baloons
#define ycen 105  //y cordinate center of ellipse
#define xcen -79  // x cordinate center of ellipse
#define sr 56    // radius of smaller balloon
#define br 89   // radius of bigger radius
#define late 0  // delay time

struct screen
{
    int c[7];       // c for counters i.e i,j,k,l,m
    int asize[7];   //for storing the size of bubble in the screen
    int indi:3;     // indicates which bubbles are selected(animated) at that instant + it's size is 3 bits ie can store 2^3 ints value from 0 to 8
};
typedef struct screen s;
int rnd(int higher_limit) //for random number generator
{
    time_t raw_time = time(NULL);
    srand(raw_time);
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
    switch(s1->indi)
    {
        case 4: fillellipse(xcen+s1->c[4],ycen,s1->asize[4],s1->asize[4]);
        case 3: fillellipse(xcen+s1->c[3],ycen,s1->asize[3],s1->asize[3]);
        case 2: fillellipse(xcen+s1->c[2],ycen,s1->asize[2],s1->asize[2]);
        case 1: fillellipse(xcen+s1->c[1],ycen,s1->asize[1],s1->asize[1]);
        case 0: fillellipse(xcen+s1->c[0],ycen,s1->asize[0],s1->asize[0]);
        delay(late);
    }

}

int main()
{
    int gd = DETECT, gm;
    //initgraph(&gd, &gm, "C:\\TC\\BGI");
    initwindow(800,600,"Poploon");
    s s1;      // decalre a variable s1 of struct screen
    here1:
    setbkcolor(WHITE);
    setcolor(BLUE);
    for(s1.c[0]=1,s1.asize[0]=rd(),s1.indi=0;;s1.c[0]++)
    {
       if(s1.c[0]%GAP==0)
       {
           for(s1.c[1]=1,s1.asize[1]=rd(),s1.indi=1;;s1.c[1]++,s1.c[0]++)
           {
               if(s1.c[1]%GAP==0)
               {
                   for(s1.c[2]=1,s1.asize[2]=rd(),s1.indi=2;;s1.c[2]++,s1.c[1]++,s1.c[0]++)
                    {
                        if(s1.c[2]%GAP==0)
                          {
                              for(s1.c[3]=1,s1.asize[3]=rd(),s1.indi=3;;s1.c[3]++,s1.c[2]++,s1.c[1]++,s1.c[0]++)
                              {
                                  if(s1.c[3]%GAP==0)
                                  {
                                      for(s1.c[4]=1,s1.asize[4]=rd(),s1.indi=4;;s1.c[4]++,s1.c[3]++,s1.c[2]++,s1.c[1]++,s1.c[0]++)
                                        {
                                              bu_gen(&s1);

                                              if(s1.c[0]==(getmaxx()-xcen+br))
                                                goto here1;


                                        }
                                        }
                                              bu_gen(&s1);
                                        }
                                        }
                                              bu_gen(&s1);
                                        }
                }
                                              bu_gen(&s1);

            }
        }
                                              bu_gen(&s1);
    }
   getch();
   closegraph();
   return 0;
}
