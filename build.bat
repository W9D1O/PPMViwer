gcc -o main.exe main.c  -ggdb -Wall -Wextra -I include -L lib -lraylib -lgdi32 -lwinmm
::clang -o main.exe main.c  -I include -L lib -lraylib -lgdi32 -lwinmm
