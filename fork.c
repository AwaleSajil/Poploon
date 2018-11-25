#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <conio.h>
#include <dos.h>
#include <time.h>
void main()
{
    POINT m;
    for(;;)
    {
        GetCursorPos(&m);
        printf("%d %d",m.x,m.y);
        delay(50);
    }
}
