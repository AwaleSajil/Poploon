#include <stdio.h>
#include<graphics.h>
#include<conio.h>
#include<dos.h>
#include<time.h>
#define GAP 180   // gap between consecutive baloons
#define ycen 105  //y cordinate center of ellipse
#define xcen -79  // x cordinate center of ellipse
#define sr 56    // radius of smaller balloon
#define br 89   // radius of bigger radius
#define late 4  // delay time

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
void more_bubble()
{

}
int main()
{
    int gd = DETECT, gm, i=1, j=1, k=1, l=1, m=1;
    //initgraph(&gd, &gm, "C:\\TC\\BGI");
    initwindow(800,600,"Poploon");

    int asize[7];       //for storing the size of bubble in the screen

    here1:
    setbkcolor(WHITE);
    setcolor(BLUE);
    for(asize[0]=rd();;i++)
    {
       if(i%GAP==0)
       {
           for(asize[1]=rd();;j++,i++)
           {
               if(j%GAP==0)
               {
                   for(asize[2]=rd();;k++,i++,j++)
                    {
                        if(k%GAP==0)
                          {
                              for(asize[3]=rd();;l++,k++,j++,i++)
                              {
                                  if(l%GAP==0)
                                  {
                                      for(asize[4]=rd();;m++,l++,k++,j++,i++)
                                        {
                                              fillellipse(xcen+m, ycen,asize[4],asize[4]);
                                              fillellipse(xcen+l,ycen,asize[3],asize[3]);
                                              fillellipse(xcen+k,ycen,asize[2],asize[2]);
                                              fillellipse(xcen+j,ycen,asize[1],asize[1]);
                                              fillellipse(xcen+i,ycen,asize[0],asize[0]);
                                              delay(late);

                                              if(i==(getmaxx()+xcen+br))
                                                goto here1;


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
