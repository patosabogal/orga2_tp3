/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "screen.h"

void print(const char * text, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    int i;
    for (i = 0; text[i] != 0; i++) {
        p[y][x].c = (unsigned char) text[i];
        p[y][x].a = (unsigned char) attr;
        x++;
        if (x == VIDEO_COLS) {
            x = 0;
            y++;
        }
    }
}

void print_hex(unsigned int numero, int size, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    int i;
    char hexa[8];
    char letras[16] = "0123456789ABCDEF";
    hexa[0] = letras[ ( numero & 0x0000000F ) >> 0  ];
    hexa[1] = letras[ ( numero & 0x000000F0 ) >> 4  ];
    hexa[2] = letras[ ( numero & 0x00000F00 ) >> 8  ];
    hexa[3] = letras[ ( numero & 0x0000F000 ) >> 12 ];
    hexa[4] = letras[ ( numero & 0x000F0000 ) >> 16 ];
    hexa[5] = letras[ ( numero & 0x00F00000 ) >> 20 ];
    hexa[6] = letras[ ( numero & 0x0F000000 ) >> 24 ];
    hexa[7] = letras[ ( numero & 0xF0000000 ) >> 28 ];
    for(i = 0; i < size; i++) {
        p[y][x + size - i - 1].c = hexa[i];
        p[y][x + size - i - 1].a = attr;
    }
}

void print_int(unsigned int n, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    if( n > 9 ) {
      int a = n / 10;
      n -= 10 * a;
      print_int(a,x-1,y,attr);
    }
    p[y][x].c = '0'+n;
    p[y][x].a = attr;
}

void pintar_rectangulo(unsigned int x, unsigned int y, unsigned int cant_x, unsigned int cant_y, unsigned short attr){
    unsigned int temp_x;

    while ( y < cant_y){
        temp_x = x;
        while (temp_x < cant_x){
            print(" ", temp_x, y, attr);
            temp_x++;
        }
        y++;
    }
}

void screen_inicializar(){
    pintar_rectangulo(0, 0, 80, 1, C_FG_WHITE | C_BG_BLACK);
    pintar_rectangulo(0, 1, 80, 45, C_FG_WHITE | C_BG_LIGHT_GREY);
    pintar_rectangulo(0, 45, 80, 50, C_FG_WHITE | C_BG_BLACK);

    pintar_rectangulo(48, 45, 54, 50, C_FG_WHITE | C_BG_RED);
    pintar_rectangulo(54, 45, 60, 50, C_FG_WHITE | C_BG_BLUE);

    print("<A     B>", 13, 46, C_FG_WHITE | C_BG_BLACK);
    print("vidas", 41, 46, C_FG_WHITE | C_BG_BLACK);
    print("vidas", 62, 46, C_FG_WHITE | C_BG_BLACK);

    actualizar_display_vidas();
    actualizar_display_puntos();

}

void actualizar_display_vidas(){
    print_int(GAME.vidas_A, 43, 48, C_FG_WHITE | C_BG_BLACK);
    print_int(GAME.vidas_B, 64, 48, C_FG_WHITE | C_BG_BLACK);
}

void actualizar_display_puntos(){
    print_int(GAME.puntos_A, 51, 47, C_FG_WHITE | C_BG_RED);
    print_int(GAME.puntos_B, 57, 47, C_FG_WHITE | C_BG_BLUE);
}