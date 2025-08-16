#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define ALTO 1080
#define ANCHO 1920
#define BUFFER (ALTO*ANCHO)*3

void LoadImg(unsigned char img[],int len,char header[]){
  for(int j = len; j < BUFFER-3;j += 3){
    img[j] = header[j];
    img[j+1]=header[j];
    img[j+2]=header[j];
    }
}
int main()
{
  unsigned char* img = (unsigned char*)malloc(BUFFER);
  char header[32];
  sprintf(header, "P6\n%d %d\n255\n", ANCHO, ALTO);
  int len = sizeof(header)/sizeof(header[0])-1;
  printf("%d\n",len);
  FILE *f;
  f = fopen(".\\prueba.ppm","wb");
  if(f == NULL){
    printf("Algo no esta funcionando\n");
    exit(1);
  }
  LoadImg(img,len,header);
  fwrite(header, 1,strlen(header), f);  
  fwrite(img,1,BUFFER,f);
  fclose(f);
  return 0;
}
