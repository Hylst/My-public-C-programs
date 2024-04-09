#include <math.h>
#include <dos.h>
#include <stdlib.h>
#include <conio.h>

typedef unsigned char byte;

void Initialize320x200x256(void)
{
    asm { mov ax, 13h
          int 10h
    }
}

void BackToText(void)
{
    asm { mov ax, 03h
          int 10h
    }
}

          
void Putpixel(int x, int y, byte color)
{
    byte far *vram;

    vram = (byte far *) MK_FP(0xA000, 320 * y + x);
    *vram = color;
}
void clean(void)
{
    asm {
        push 0a000h
        pop es
        xor  di,di
        mov  ax,di
        mov  cx,32000
        rep  stosw
    }
}
void main(void)
{

    int x , y;
    float k ,j , n, i;
    int xi[629];
    int yi[629];
    int zi[629];

    Initialize320x200x256();

    for(k = 0, j = 0; k < 2 * M_PI; k += 0.01, j++)
    {
        xi[j] = 90 * cos(k);
        yi[j] = 90 * sin(k);
        Putpixel(xi[j] + 160, yi[j] + 100, 1);
    }
    for(;;)
    {

        for(k = 0; k < 630; k++)
//        for(k = 0; k < 2 * M_PI; k+= 0.01)
        {
         clean();
            for(j = 0; j < 2 * M_PI; j += 0.1 * M_PI)
            {
                 for(i = 0; i < 630 ; i++)
                 {
/*                  x=160+xi[i]*cos(k+j);
                  y=100+yi[i]*sin(k+j);*/
                  x=160+xi[((int) (k+i)) % 630]*cos(j);
                  y=100+yi[((int) (k+i)) % 630]*sin(j);
                  Putpixel(x ,y , 2);

                 }
            }
         }
    }


    getch();
    BackToText();
}
