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
    actualizar_dislpay_estatico();
    escribir_nombre_grupo();
    actualizar_display_vidas();
    actualizar_display_puntos();
    actualizar_display_cursores();
    actualizar_display_debug_mode();
}

void escribir_nombre_grupo(){
    print("Lo bueno de las arvejas", 28, 0, C_FG_WHITE | C_BG_BLACK);
}

void actualizar_dislpay_estatico(){
	pintar_rectangulo(0, 0, 80, 1, C_FG_WHITE | C_BG_BLACK);
    pintar_rectangulo(0, 1, 80, 45, C_FG_WHITE | C_BG_LIGHT_GREY);
    pintar_rectangulo(0, 45, 80, 50, C_FG_WHITE | C_BG_BLACK);

    pintar_rectangulo(48, 45, 54, 50, C_FG_WHITE | C_BG_RED);
    pintar_rectangulo(54, 45, 60, 50, C_FG_WHITE | C_BG_BLUE);

    print("<A     B>", 13, 46, C_FG_WHITE | C_BG_BLACK);
    print("vidas", 41, 46, C_FG_WHITE | C_BG_BLACK);
    print("vidas", 62, 46, C_FG_WHITE | C_BG_BLACK);
}

void actualizar_display_vidas(){
    print_int(GAME.A.vidas, 43, 48, C_FG_WHITE | C_BG_BLACK);
    print_int(GAME.B.vidas, 64, 48, C_FG_WHITE | C_BG_BLACK);
}

void actualizar_display_puntos(){
    print_int(GAME.A.puntos, 51, 47, C_FG_WHITE | C_BG_RED);
    print_int(GAME.B.puntos, 57, 47, C_FG_WHITE | C_BG_BLUE);
}

void actualizar_display_cursores(){
    print("*", GAME.A.x, GAME.A.y, C_FG_WHITE | C_BG_RED);
    if (GAME.A.x == GAME.B.x && GAME.A.y == GAME.B.y){
        print("*", GAME.B.x, GAME.B.y, C_FG_WHITE | C_BG_CYAN);
    } else {
        print("*", GAME.B.x, GAME.B.y, C_FG_WHITE | C_BG_BLUE);
    }
}

void actualizar_display_debug_mode(){
	if (DEBUG_MODE){
        print("Y", 79, 0, C_FG_WHITE | C_BG_BLACK);
    }
}

void mostrar_pantallita(unsigned short virus){
    pintar_rectangulo(25,7,54,42,C_FG_WHITE | C_BG_BLACK);
    pintar_rectangulo(26,8,53,41,C_FG_WHITE | C_BG_LIGHT_GREY);
    if(virus==0){
        pintar_rectangulo(26,8,53,9,C_FG_WHITE | C_BG_RED);
        print("virus A", 26, 8, C_FG_WHITE | C_BG_RED);
    }
    else{
        pintar_rectangulo(26,8,53,9,C_FG_WHITE | C_BG_BLUE);
        print("virus B", 26, 8, C_FG_WHITE | C_BG_BLUE);
    }

    unsigned int* asd = (unsigned int *) 0x27000;
    print("eax", 27, 10, C_FG_BLACK | C_BG_LIGHT_GREY);
    print_hex(*asd,8, 31, 10, C_FG_WHITE | C_BG_LIGHT_GREY);
    
    // print_registro32("ebx"27,12);
    // print_registro32("ecx",27,14);
    // print_registro32("edx",27,16);
    // print_registro32("esi",27,18);
    // print_registro32("edi",27,20);
    // print_registro32("ebp",27,22);
    // print_registro32("esp",27,24);
    // print_registro32("eip",27,26);
    // print_Cregistro("cd",28,28);
    // print_Cregistro("ds",28,30);
    // print_Cregistro("es",28,32);
    // print_Cregistro("fs",28,34);
    // print_Cregistro("gs",28,36);
    // print_Cregistro("ss",28,38);
    // print_Eflags("eflags",27,40);
}

// void print_registro32(const char* registro,short x,short y){
//     register int i asm("eax");
//     print(registro, x, y, C_FG_BLACK | C_BG_LIGHT_GREY);
//     print_hex(i,8, x+4, y, C_FG_WHITE | C_BG_LIGHT_GREY);
// }

// void print_Cregistro(const char*registro, short x, short y){
//     register int i asm("eax");
//     print(registro, x, y, C_FG_BLACK | C_BG_LIGHT_GREY);
//     print_hex(i,8, x+4, y, C_FG_WHITE | C_BG_LIGHT_GREY);
// }
