#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define ALTO 5
#define ANCHO 8
#define BUFFER (ALTO*ANCHO)*3

void LoadImg(unsigned char img[],int len,char header[]){
  char img2[] = {
    0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0, 3,  3,  3,  3,  0,  0,  7,  7,  7,  7,  0,  0, 11, 11, 11, 11,  0,  0, 15, 15, 15, 15,  0,
    0, 3,  0,  0,  0,  0,  0,  7,  0,  0,  0,  0,  0, 11,  0,  0,  0,  0,  0, 15,  0,  0, 15,  0,
    0, 3,  3,  3,  0,  0,  0,  7,  7,  7,  0,  0,  0, 11, 11, 11,  0,  0,  0, 15, 15, 15, 15,  0,
    0, 3,  0,  0,  0,  0,  0,  7,  0,  0,  0,  0,  0, 11,  0,  0,  0,  0,  0, 15,  0,  0,  0,  0,
  };
  int len2 = sizeof(img2)/sizeof(img2[0]);
  for(int j = len; j < len2-3;j += 3){
    img[j] =  img2[j];
    img[j+1]= img2[j+1];
    img[j+2]= img2[j+2];
    }
  
}
int main()
{
  unsigned char* img = (unsigned char*)malloc(BUFFER);
  char header[32];
  sprintf(header, "P6\n%d %d\n16\n", ANCHO, ALTO);
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
