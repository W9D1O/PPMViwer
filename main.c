#include <stdio.h>
#include <stdbool.h>
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
void DrawImg2(unsigned char buffer[],Color c,int w,int h,int ini,int size){
  int x = 0;
  int y = 0;
  for(int i = ini+1; i < size-3;i+=3){
    c.a = 255;
    c.r = buffer[i+0];
    c.b = buffer[i+2];
    c.g = buffer[i+1];
      DrawPixel(ANCHO/2-(w/2)+x,ALTO/2-(h/2)+y, c);
      x ++;
    if( x == w){
      //printf("%d soy X. %d. Soy W",x,w);
      y ++;
      x = 0;
    }
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

/*Busca el caracter '\0', el cual es usado para*
 * medir la longitud re una cadena.            */
int LenChar(char c[]){
  int len = 0;
  while(c[len] != '\0'){
    len ++;
  }
  return len;
}


/* Separea  la cabecera, corrobora que el archivo sea PPM y devuelve*
 * el ancho y alto de la imagen.                                    */
void CutHeader(unsigned char header[],int *width,int *height,int len){
  char nm[4];
  char mc[4];
  char auxw[256];
  char auxh[256];
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
      auxw[i-cont] = header[i];
      auxw[i-cont+1] = '\0';
    }else{
      if(saltos > 0 && saltos  < 2 && esp == ' ' && header[i] != esp){
        auxh[i-cont] = header[i];
        auxh[i-cont+1] = '\0';
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
  *width = CharToInt(auxw,LenChar(auxw)-1);
  *height = CharToInt(auxh,LenChar(auxh)-1);
}

/*Aumenta o adisminuye el zoom de*
 * la imagen.                    */
void ImgZoom(float* zoom){
    if(IsKeyPressed(KEY_Z)){
      *zoom += 0.1f;

    }else {
      if(IsKeyPressed(KEY_M)){
        *zoom -= 0.1f;
      }
    }
}

/*Calcula la cantidad el zoom inicial tomando*
* en acuenta las dimensiones de la ventana.  */
void InitZoom(float *zoom,int width,int height){
  if(ANCHO < ALTO  && height > ALTO){
    *zoom = (float)ALTO / height;
  }else{
    if(ANCHO > ALTO && width > ANCHO){
      *zoom = (float)ANCHO/width;
    }
  }

}

/*Retorna el tamaño del archivo*/
uint32_t SizeFile(FILE *f){

  //Voy al final del archivo
  fseek(f,0L,SEEK_END);
  //Guardo la longitud del archivp
  uint32_t size = ftell(f);
  //Vuelvo al inicio del archivo
  fseek(f,0L, SEEK_SET);
  return size;
}

/*Controla que el programa haya*
* recibido algún argumento      */
int arg(int argc){

  if(argc > 1 && argc < 3){
    return 0;
  }else{
    if(argc < 2){
      printf("ERROR: No se encontro el archivo.\n");
      printf("El modo de uso es: .\\vppm ejemplo.ppm\n");
      exit(1);
    } else if(argc > 2){
      printf("ERROR: Demaciados argumentos.\n");
      printf("El modo de uso es: .\\vppm ejemplo.ppm\n");
      exit(1);
    }
  }
  return -1;
}

int main(int argc, char* argv[]){

  arg(argc);
  FILE *f;
  f = fopen(argv[1],"rb");
  if(f == NULL){
    printf("Archivo no encontrado\n");
    exit(1);
  }
  uint32_t size = SizeFile(f);
  unsigned char* buffer = (unsigned char*)malloc(size);
  fread(buffer,size,1,f);
  fclose(f);
  Camera2D cam = {
    .zoom = 1.f,
    .offset = {
      .x = (float) ANCHO/2,
      .y = (float) ALTO/2
    },
    .target = {
      .x = (float)ANCHO/2,
      .y = (float)ALTO/2,
    },
    .rotation = 0,
  };
  // Guardo la longitud del heder de la imagen.
  int lh = LenHeader(buffer);
  int sw = 0;
  int sh = 0;
  Color c;
  CutHeader(buffer,&sw,&sh, lh);
  InitZoom(&cam.zoom,sw,sh);
  printf("%d %d\n",size,lh);
  InitWindow(ANCHO,ALTO, "vamos a ver que es lo que pasa");
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    BeginMode2D(cam);
    ImgZoom(&cam.zoom);
    DrawImg2(buffer,c,sw,sh,lh,size);
    EndMode2D();
    EndDrawing();
  }
  CloseWindow();

  return 0;
}
