#include < stddef.h >
#include < math.h   >
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include " type.h   "
#include "_standar.h"
#include "_wdpmi.h"
#include "_dpmi.h"
#include "_vesa.h"

#ifdef __WATCOM__
void waitvr (void);
#pragma aux waitvr =\
	"mov dx,3dah",\
	"wait1:",\
	"in al,dx",\
	"test al,8",\
	"jnz wait1",\
	"wait2:",\
	"in al,dx",\
	"test al,8",\
	"jz wait2",\
	modify [al dx];

#endif

#ifndef M_PI
#define M_PI 3.141592653589793238462643383279502884
#endif
long int fsize(FILE * f)
{
  long int res,cur=ftell(f);
  fseek(f,0,SEEK_END);
  res=ftell(f);
  fseek(f,cur,SEEK_SET);
  return res;
}

int mode;


void set_vgamode(int a)
{
    __dpmi_regs regs;
    regs.x.ax = a;
    __dpmi_int(0x10,&regs);
    return;
}
void error(char *tx,int code)
{
    set_vgamode(0x3);
    fprintf(stderr,"%s code %d\n",tx,code);
    exit(-1);
}
void main(void)
{
    if (VBE_get_OEM_infos())
    {
    mode = VBE_get_mode_by_infos(320,200,16);
	// Set the mode and enable LFB mode
	if (VBE_set_vesa_mode(mode))
	{
	    if (VESA.Linear)
	    {
     dword * rayons,* taille,modules;
     short int x,y,c,d,t,e;
     float *sintab,*biztab,dec,valdec,vade2;
     word * video=(word *)VESA.Video;
     {
        FILE * r, * t;
        dword rs,ts;
        r=fopen("circles.ntl","rb");
        t=fopen("circndx.ntl","rb");
        rs=fsize(r);
        ts=fsize(t);
        rayons=malloc(rs);
        taille=malloc(ts);
        fread(rayons,1,rs,r);
        fread(taille,1,ts,t);
        fclose(r);
        fclose(t);
        modules=ts/sizeof(dword);
     }

     sintab=malloc(360*sizeof(float));
     biztab=malloc(360*sizeof(float));
     if (sintab==NULL)
        exit ( EXIT_FAILURE ) ;
      for(x=0;x<360;x++)
      {
      sintab[x]=fabs(sin(M_PI/180*x));
      biztab[x]=fabs(2*cos(M_PI/180*x)+sin(M_PI/180*4*x));
      }
      e=0;
      while(!kbhit())
      {
      d=0;
      t=e++;
      waitvr();
      for(x=0;x<100;x++)
        {
        c=31*sintab[t=(t+1)%360];
        valdec=360*sintab[t];
        vade2=120*biztab[t];
        dec=360.0/taille[x];
        for(y=0;y<taille[x];y++)
         {
           valdec=fmod(valdec+dec,360);
           vade2=fmod(vade2+dec,360);
           video[rayons[d++]>>1]=c+((int)(31*sintab[(int)valdec])<<11)
           +((int)(21*biztab[(int)vade2])<<5);
         }
        }
      }

      free(sintab);

    getch();

	    }
	    set_vgamode(0x3);
	}
	else
	{
	   set_vgamode(0x3);
	   printf("Cannot initialize mode %x\n",mode);
	   getch();
	}
    }
    exit ( EXIT_SUCCESS ) ;
}
