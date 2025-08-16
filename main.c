#include <stdio.h>
#include <stdbool.h>
//#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include ".\\include\\raylib.h"

#define ALTO 600
#define ANCHO 800

/*Recibe una base y un exponente   *
 * y realiza su respectava potencia*/
int pot(int pos,int base){
  int r = 1;
  if(pos == 0) return 1;
  for(int i = 1; i <= pos; i++){
    r *= base; 
  }
  return r;
}

/* Transforma una cadena de carácteres   *
 * a su represantación en formato decimal*/
int CharToInt(char n[],int len){
  int r = 0;
  int base = 10;
  for(int i = len;i > -1; i-- ){
   r += (n[i] - 48)*pot(len-i,base);
  }
  return r;
}


/*Recibe un array de caracteres el cual es usado para representar * 
 * los colores en Formato RGB, pinta pixel por pixel la imagen    *
 * en la ventana                                                  */
void DrawImg(unsigned char buffer[],int fin,Color c,int x,int y,int w,int h,int size){

  for(int i = fin; i < (size-3);i+=3){
    c.a = 255;
    c.r = buffer[i];
    c.b = buffer[i+1];
    c.g = buffer[i+2];
    if(x != 0 && x % w == 0){
      printf("%d soy X. %d. Soy W",x,w);
      y ++;
      x = 0;
    }
    DrawPixel(ANCHO/2-w/2+x,ALTO/2-h/2+y, c);
    x ++;
  }
}

/*Corrobora si el archivo cargado tiene el numero *
 * magico "P&" . Dervuelve true en caso de encon  *
 * trarlo.                                        */
bool IsPPM(char nm[],char mc[]){
  char anm[] = "P6";
  char amc[] = "255";
  for(int i = 0;i < 3; i++){
    if(i < 2){
      if(nm[i] != anm[i] && mc[i] != amc[i]){
        return false;
      }else{
        if(amc[i] != mc[i]){
          return false;
        }
      }
    }
  }
  return true;
}

/*Devuelve la dimension lógica del array*/
int LenHeader(unsigned char header[]){
  int saltos = 0;
  int len = 0;
  while(saltos < 3){
    if(header[len] == '\n'){
      saltos++;
    }
    len++;
  }
  return len-1;
}

/* Separea  la cabecera, corrobora que el archivo sea PPM y devuelve*
 * el ancho y alto de la imagen.                                    */
void CutHeader(unsigned char header[],char width[],char height[],int len){
  char nm[4];
  char mc[4];
  int saltos = 0;
  char esp;
  int cont = 0;
  for(int i=0;i < len;i++){
    if(header[i] == '\n'){
      saltos ++;
      cont = i+ 1;;
    }
    if(header[i] == ' '){
      esp = header[i];
      cont = i + 1;
    }

    if(saltos < 1){
      nm[i] = header[i];
    }
    if(saltos > 0 && saltos  < 2 && esp != ' ' && header[i] != '\n'){
      width[i-cont] = header[i];
    }else{
      if(saltos > 0 && saltos  < 2 && esp == ' ' && header[i] != esp){
        height[i-cont] = header[i];
      }
    }

    if(saltos < 3 && esp == ' ' && header[i] != '\n'){
      mc[i-cont] = header[i];
    }
  }
  if(!IsPPM(nm,mc)){
    printf("El archivo ingresado no corresponer a el formato PPM 6\n");
    exit(1);
  }
}

int main(){
  FILE *f;
  f = fopen("simple.ppm","rb");
  unsigned char header[32];
  fread(header,sizeof(header),1,f);
  if(f == NULL){
    printf("Archivo no encontrado\n");
    exit(1);
  }
  int fin = LenHeader(header);
  Color c;
  int x = 0;
  int y = 0;
  char height[4];
  char width[4];
  int len = sizeof(header)/sizeof(header[0]);
  CutHeader(header,width,height, len);
  printf("%s %s\n",width,height);
  int lw = sizeof(width)/sizeof(width[0]) -2;
  int lh = sizeof(width)/sizeof(width[0]) -2;
  int w = CharToInt(width, lw);
  int h = CharToInt(height, lh);
  Camera2D cam = {
      .offset = {
      .x = 0,//(float) ANCHO/2-(float)w/2,
      .y =(float) ALTO/2-(float)h/2,
    },
    .target = {
      .x = (float)ANCHO/2-(float)w/2+x,
      .y = (float)ALTO/2-(float)h/2+y

    },
    .rotation = 0,
    .zoom = 1.f,

  };
  uint32_t size = (w*h + fin )*3;
  unsigned char* buffer = (unsigned char*)malloc(size);
  /*if(buffer == NULL){
    printf("Algo salio mal\n");
    exit(1);
  }*/
  unsigned int leidos = fread(buffer,size,1,f);
  printf("esoty en leidos %d\n",leidos);
  fclose(f);
 InitWindow(ANCHO,ALTO, "vamos a ver que es lo que pasa");
 while (!WindowShouldClose()) {
   BeginDrawing();
   ClearBackground(RAYWHITE);
    if(IsKeyPressed(KEY_Z)){
      cam.zoom += 0.5f;
      
    }else {
      if(IsKeyPressed(KEY_MINUS)){
        cam.zoom -= 0.5f;
      }
    }
    BeginMode2D(cam);
    DrawImg(buffer,fin,c,x,y,w,h,size);
    EndMode2D();
   EndDrawing();
 }
 CloseWindow();
 
  return 0;
  
}
