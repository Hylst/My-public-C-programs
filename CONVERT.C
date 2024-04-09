#include <stdio.h>
void main(void)
{
    FILE *f1,*f2;
    unsigned long int i;
    char a,b;
    f1=fopen("draget00.16b","rb");
    f2=fopen("draget00.int","wb");
    fseek(f1,4,SEEK_SET);
    for(i=0;i<256000;++i)
    {
        a=fgetc(f1);
        b=fgetc(f1);
        fputc(b,f2);
        fputc(a,f2);
    }
    fclose(f1);
    fclose(f2);
}
