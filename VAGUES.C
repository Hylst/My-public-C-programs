#include <stdio.h>
#include <conio.h>
#include "vesagfx.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795028841971693993
#endif
#define left 150
#define right 340
#define top 60
#define bottom 290
#define picwidth 640
#define picheight 400

#define scrwidth (right-left)
#define scrheight (bottom-top)
#define maxdiag (sqrt(pow(scrwidth/2,2)+pow(scrheight/2,2)))
#define picsize (picwidth*picheight)
#define scrsize (scrwidth*scrheight)
#define centre_x ((left+right)/2)
#define centre_y ((top+bottom)/2)

unsigned short int rayon[scrsize],screen[picsize],tut[scrsize],phase[scrsize];
unsigned long int taby[400];
float tc[1260],ts[1260];
void loadscreen(void);
void calc_tables(void);

void main(void)
{
    unsigned long int v;
    signed int x,y,n,i;
    loadscreen();
    calc_tables();
    graph();
    for(i=n=0;(n<629)&&(kbhit()==0);n+=2,i+=50-(630&(-(i>=(630-50)))))
    {
     cls();
     for(v=0;v<scrsize;v++)
     {
      x=(signed int)centre_x+(1.5+tc[i])*rayon[v]*tc[phase[v]+n];
      y=(signed int)centre_y+(1.5+tc[i])*rayon[v]*ts[phase[v]+n];
      if (((unsigned int)x<640)&&((unsigned int)y<400))
      vram[x+taby[y]]=tut[v];
     }
    }
    getch();
    text();
}

void calc_tables(void)
{
    signed int x,y,k;
    float a,p;
    for(k=0,a=0;a<4*M_PI;a+=0.01,k++)
    {
     tc[k]=cos(a);
     ts[k]=sin(a);
    }
    for(k=0;k<400;k++)
    {
     taby[k]=k*640;
    }
    k=0;
    for(y=top;y<bottom;y++)
    {
     for(x=left;x<right;x++)
     {
      a=sqrt(pow(centre_x-x,2)+pow(centre_y-y,2));
      p=-(centre_x-x)/a;
      p=acos(p);
      if (y<=(unsigned short int)centre_y) p=-p;
      if (p<0) p=p+2*M_PI;
      rayon[k]=a;
      phase[k]=100*p;
      tut[k]=screen[x+taby[y]];
      k++;
     }
    }
}
void loadscreen(void)
{
    FILE *f;
    f=fopen("draget00.int","rb");
    fread(screen,4,128000,f);
    fclose(f);
}