#include <stdio.h>
#include<graphics.h>
#include<conio.h>
#include<dos.h>
#include<time.h>
#define GAP 160   // gap between consecutive baloon
#define ycen 65  //y cordinate center of ellipse
#define xcen 79  // x cordinate center of ellipse
#define sr 56    // radius of smaller balloon
#define br 89   // radius of bigger radius
#define late 21  // delay time

int rnd(int higher_limit) //for random number generator
{
    time_t raw_time = time(NULL);
    srand(raw_time);
    return (rand()%higher_limit);
}
int rd()
{
    if(rnd(2))
        return br;
    else
        return sr;
}
int main()
{
    int gd = DETECT, gm, i, j, k, l, m;
    initgraph(&gd, &gm, "C:\\TC\\BGI");
    int asize[7];       //for storing the size of bubble in the screen
    setbkcolor(WHITE);
    setcolor(BLUE);
    for(i=1,asize[0]=rd();;i++)
    {
       if(i%GAP==0)
       {
           for(j=1,asize[1]=rd();;j++,i++)
           {
               if(j%GAP==0)
               {
                   for(k=1,asize[2]=rd();;k++,i++,j++)
                    {
                        if(k%GAP==0)
                          {
                              for(l=1,asize[3]=rd();;l++,k++,j++,i++)
                              {
                                  if(l%GAP==0)
                                  {
                                      for(m=1,asize[4]=rd();;m++,l++,k++,j++,i++)
                                        {
                                              fillellipse(xcen+m, ycen,asize[4],asize[4]);
                                              fillellipse(xcen+l,ycen,asize[3],asize[3]);
                                              fillellipse(xcen+k,ycen,asize[2],asize[2]);
                                              fillellipse(xcen+j,ycen,asize[1],asize[1]);
                                              fillellipse(xcen+i,ycen,asize[0],asize[0]);
                                              delay(late);
                                        }
                                        }
                                              fillellipse(xcen+l,ycen,asize[3],asize[3]);       //4th bubble 1st part
                                              fillellipse(xcen+k,ycen,asize[2],asize[2]);       // 3rd bubble 2nd part
                                              fillellipse(xcen+j,ycen,asize[1],asize[1]);       //2nd bubble 3rd part
                                              fillellipse(xcen+i,ycen,asize[0],asize[0]);       //1st bubble 4th part
                                              delay(late);
                                        }
                                        }
                                              fillellipse(xcen+k,ycen,asize[2],asize[2]);       //3rd bubble 1st part
                                              fillellipse(xcen+j,ycen,asize[1],asize[1]);       //2nd bubble 2nd part
                                              fillellipse(xcen+i,ycen,asize[0],asize[0]);       //1st bubble 3rd part
                                              delay(late);
                                        }
                }
                                              fillellipse(xcen+j,ycen,asize[1],asize[1]);                   //2nd bubble 1st part
                                              fillellipse(xcen+i,ycen,asize[0],asize[0]);                   //1st bubble 2nd part
                                              delay(late);

            }
        }
                                              fillellipse(xcen+i,ycen,asize[0],asize[0]);delay(late);       //1st bubble 1st part
    }
   getch();
   closegraph();
   return 0;
}
