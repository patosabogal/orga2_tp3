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

void print_char(unsigned char c, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
        p[y][x].c = (unsigned char) c;
        p[y][x].a = (unsigned char) attr;
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

void pintar_rectangulo(unsigned int x, unsigned int y, unsigned int ancho, unsigned int alto, unsigned short attr){
    unsigned int temp_x;
    unsigned int cant_y = y + alto;
    unsigned int cant_x = x + ancho;

    while ( y < cant_y){
        temp_x = x;
        while (temp_x < cant_x){
            print(" ", temp_x, y, attr);
            temp_x++;
        }
        y++;
    }
}

void actualizar_display_relojJ(){
    unsigned int i = 0;
    while(i<CANT_TAREAS_J){
        if(GAME.js[A].tareas[i].vivo){
            print_char(GAME.js[A].tareas[i].reloj,2+2*i,46,C_FG_WHITE | C_BG_BLACK);
        }
        else
            print_char(' ',2+2*i,46,C_FG_WHITE | C_BG_BLACK);
        if(GAME.js[B].tareas[i].vivo) 
            print_char(GAME.js[B].tareas[i].reloj, 24+2*i,46,C_FG_WHITE | C_BG_BLACK);
        else
            print_char(' ', 24+2*i,46,C_FG_WHITE | C_BG_BLACK);
        i++;
    }
}

void actualizar_display_relojS(){
    unsigned int i = 0;
    while(i<CANT_H){
        if(GAME.iniciales[i].vivo) 
            print_char(GAME.iniciales[i].reloj,2+2*i,48,C_FG_WHITE | C_BG_BLACK);
        i++;
    }
}

void screen_inicializar(){
    actualizar_display_estatico();
    escribir_nombre_grupo();
    actualizar_display_vidas();
    actualizar_display_puntos();
    actualizar_display_mapeadas();
    actualizar_display_tareas();
    actualizar_display_debug_mode();
    actualizar_display_relojJ();
    actualizar_display_relojS();
    actualizar_display_cursores();
}

void escribir_nombre_grupo(){
    print("Lo bueno de las arvejas", 28, 0, C_FG_WHITE | C_BG_BLACK);
}

void actualizar_display_estatico(){
	pintar_rectangulo(0, 0, 80, 1, C_FG_WHITE | C_BG_BLACK);
    pintar_rectangulo(0, 1, 80, 44, C_FG_WHITE | C_BG_LIGHT_GREY);
    pintar_rectangulo(0, 45, 80, 5, C_FG_WHITE | C_BG_BLACK);

    pintar_rectangulo(48, 45, 6, 5, C_FG_WHITE | C_BG_RED);
    pintar_rectangulo(54, 45, 6, 5, C_FG_WHITE | C_BG_BLUE);

    print("<A     B>", 13, 46, C_FG_WHITE | C_BG_BLACK);
    print("vidas", 41, 46, C_FG_WHITE | C_BG_BLACK);
    print("vidas", 62, 46, C_FG_WHITE | C_BG_BLACK);
}

unsigned short _punto_en_rango(unsigned int pto_x, unsigned int pto_y, unsigned int x, unsigned int y, unsigned int ancho, unsigned int alto){
    return (pto_x >= x && pto_x < x + ancho && pto_y >= y && pto_y < y + alto);
}



unsigned int _color(id j){
    switch(j){
        case A:
            return C_BG_RED;
        case B:
            return C_BG_BLUE;
        default:
            return C_BG_GREEN;
    }
}


void actualizar_display_punto(unsigned int x, unsigned int y){
    //Pantalla Estatica
    if(_punto_en_rango(x,y,0,0,80,1))
        pintar_rectangulo(x, y, 1, 1, C_FG_WHITE | C_BG_BLACK);
    if(_punto_en_rango(x,y,0,1,80,44)){
        pintar_rectangulo(x, y, 1, 1, C_FG_WHITE | C_BG_LIGHT_GREY);
    }
    if(_punto_en_rango(x,y,0,45,80,5))
        pintar_rectangulo(x, y, 1, 1, C_FG_WHITE | C_BG_BLACK);

    if(_punto_en_rango(x,y,48,45,6,5))
        pintar_rectangulo(x, y, 1, 1, C_FG_WHITE | C_BG_RED);
    if(_punto_en_rango(x,y,54,45,6,5))
        pintar_rectangulo(x, y, 1, 1, C_FG_WHITE | C_BG_BLUE);

    if(_punto_en_rango(x,y,13,46,9,1))
        print("<A     B>", 13, 46, C_FG_WHITE | C_BG_BLACK);
    if(_punto_en_rango(x,y,41,46,5,1))
        print("vidas", 41, 46, C_FG_WHITE | C_BG_BLACK);
    if(_punto_en_rango(x,y,62,46,5,1))
        print("vidas", 62, 46, C_FG_WHITE | C_BG_BLACK);

    int i;
    actualizar_display_mapeadas();

    //Tareas
    for (i = 0; i < CANT_H; ++i)
    {
        tarea* _tarea = &GAME.iniciales[i];

        if (_tarea->vivo && _punto_en_rango(x,y, _tarea->x, _tarea->y,1,1)){
            print("H", _tarea->x, _tarea->y, C_FG_WHITE | _color(_tarea->virus));
        }
    }

    for (i = 0; i < CANT_TAREAS_J; ++i){
        tarea* _tareaA = &GAME.js[A].tareas[i];
        tarea* _tareaB = &GAME.js[B].tareas[i];
        if (_tareaA->vivo && _punto_en_rango(x,y, _tareaA->x, _tareaA->y,1,1)) 
            print("A", _tareaA->x, _tareaA->y, C_FG_WHITE | _color(_tareaA->virus));
        if (_tareaB->vivo && _punto_en_rango(x,y, _tareaB->x, _tareaB->y,1,1)) 
            print("B", _tareaB->x, _tareaB->y, C_FG_WHITE | _color(_tareaB->virus));
    }

    actualizar_display_cursores();   
}

void actualizar_display_vidas(){
    print_int(0, 43, 48, C_FG_WHITE | C_BG_BLACK);
    print_int(GAME.js[A].vidas, 43, 48, C_FG_WHITE | C_BG_BLACK);
    print_int(0, 64, 48, C_FG_WHITE | C_BG_BLACK);
    print_int(GAME.js[B].vidas, 64, 48, C_FG_WHITE | C_BG_BLACK);
}

int _puntos(id j){
    unsigned int puntos = 0;
    unsigned int i = 0;
    for(i = 0; i < CANT_H; i++){
        if(GAME.iniciales[i].vivo && GAME.iniciales[i].virus == j){
            puntos++;
        }
    }
    for(i = 0; i <CANT_TAREAS_J; i++){
        if(GAME.js[A].tareas[i].vivo && GAME.js[A].tareas[i].virus == j){
            puntos++;
        }
        if(GAME.js[B].tareas[i].vivo && GAME.js[B].tareas[i].virus == j){
            puntos++;
        }
    }
    return puntos;
}
void actualizar_display_puntos(){
    unsigned int puntos = _puntos(A);
    print_int(puntos, 51, 47, C_FG_WHITE | C_BG_RED);
    puntos = _puntos(B);
    print_int(puntos, 57, 47, C_FG_WHITE | C_BG_BLUE);
}



void actualizar_display_cursores(){
    print("*", GAME.js[A].x, GAME.js[A].y, C_FG_WHITE | C_BG_RED);
    if (GAME.js[A].x == GAME.js[B].x && GAME.js[A].y == GAME.js[B].y){
        print("*", GAME.js[B].x, GAME.js[B].y, C_FG_WHITE | C_BG_CYAN);
    } else {
        print("*", GAME.js[B].x, GAME.js[B].y, C_FG_WHITE | C_BG_BLUE);
    }
}

void actualizar_display_debug_mode(){
	if (DEBUG_MODE){
        print("Y", 79, 0, C_FG_WHITE | C_BG_BLACK);
    }
}

void actualizar_display_tareas(){
    int i;
    for (i = 0; i < CANT_H; ++i)
    {
        tarea* _tarea = &GAME.iniciales[i];

        if (_tarea->vivo) 
            print("H", _tarea->x, _tarea->y, C_FG_WHITE | _color(_tarea->virus));
    }

    for (i = 0; i < CANT_TAREAS_J; ++i){
        tarea* _tareaA = &GAME.js[A].tareas[i];
        tarea* _tareaB = &GAME.js[B].tareas[i];
        if (_tareaA->vivo ) 
            print("A", _tareaA->x, _tareaA->y, C_FG_WHITE | _color(_tareaA->virus));
        if (_tareaB->vivo ) 
            print("B", _tareaB->x, _tareaB->y, C_FG_WHITE | _color(_tareaB->virus));
    }
}

void actualizar_display_mapeadas(){
    int i;
    for (i = 0; i < CANT_H; ++i)
    {
        tarea* _tarea = &GAME.iniciales[i];

        if (_tarea->vivo && _tarea->map){
            if(_tarea->virus == A){
                print("A", _tarea->x_map, _tarea->y_map, C_FG_WHITE | C_BG_LIGHT_GREY);
            }else{
                print("B", _tarea->x_map, _tarea->y_map, C_FG_WHITE | C_BG_LIGHT_GREY);
            }
        }
    }

    for (i = 0; i < CANT_TAREAS_J; ++i){
        tarea* _tareaA = &GAME.js[A].tareas[i];
        tarea* _tareaB = &GAME.js[B].tareas[i];
        if (_tareaA->vivo && _tareaA->map ) print("A", _tareaA->x_map, _tareaA->y_map, C_FG_WHITE | C_BG_LIGHT_GREY);
        if (_tareaB->vivo && _tareaB->map ) print("B", _tareaB->x_map, _tareaB->y_map, C_FG_WHITE | C_BG_LIGHT_GREY);
    }
}
    

void print_registro(char* creg, unsigned int valor, unsigned int disp, unsigned int x, unsigned int y){
    print(creg, x, y, C_FG_BLACK | C_BG_LIGHT_GREY);
    print_hex(valor,disp, x+4, y, C_FG_WHITE | C_BG_LIGHT_GREY);
}


void mostrar_debug(unsigned int* registros){
    enum reg{
        cr4 = 0,
        cr3 = 1,
        cr2 = 2,
        cr0 = 3,
        gs = 4,
        fs = 5,
        es = 6,
        ds = 7,
        edi = 8,
        esi = 9,
        ebp = 10,
        ebx = 11,
        edx = 12,
        ecx = 13,
        eax = 14,
        eip = 15,
        cs = 16,
        eflags = 17,
        esp = 18,
        ss = 19
    };

    pintar_rectangulo(25,7,29,35,C_FG_WHITE | C_BG_BLACK);
    pintar_rectangulo(26,8,27,33,C_FG_WHITE | C_BG_LIGHT_GREY);

    pintar_rectangulo(26,8,27,1,C_FG_WHITE | _color(GAME.corriendo));
    if(GAME.corriendo == A) print("virus A", 26, 8, C_FG_WHITE | C_BG_RED);
    if(GAME.corriendo == B) print("virus B", 26, 8, C_FG_WHITE | C_BG_BLUE);
    if(GAME.corriendo == H) print("virus H", 26, 8, C_FG_WHITE | C_BG_GREEN);


    print_registro("eax",registros[eax],8,27,10);
    print_registro("ebx",registros[ebx],8,27,12);
    print_registro("ecx",registros[ecx],8,27,14);
    print_registro("edx",registros[edx],8,27,16);
    print_registro("esi",registros[esi],8,27,18);
    print_registro("edi",registros[edi],8,27,20);
    print_registro("ebp",registros[ebp],8,27,22);
    print_registro("esp",registros[esp],8,27,24);
    print_registro("eip",registros[eip],8,27,26);

    //Segmento
    print_registro("cs",registros[cs],4,28,28);
    print_registro("ds",registros[ds],4,28,30);
    print_registro("es",registros[es],4,28,32);
    print_registro("fs",registros[fs],4,28,34);
    print_registro("gs",registros[gs],4,28,36);
    print_registro("ss",registros[ss],4,28,38);

    //eFlags
    print_registro("efl",registros[eflags],8,27,40);

    //Control
    print_registro("cr0",registros[cr0],8,41,10);
    print_registro("cr2",registros[cr2],8,41,12);
    print_registro("cr3",registros[cr3],8,41,14);
    print_registro("cr4",registros[cr4],8,41,16);

    //Stack
    print("stack",41,26,C_FG_BLACK | C_BG_LIGHT_GREY);
    print_hex(*((unsigned int*) registros[esp]),8,41,28,C_FG_WHITE | C_BG_LIGHT_GREY);
    print_hex(*((unsigned int*) registros[esp]+1),8,41,30,C_FG_WHITE | C_BG_LIGHT_GREY);
    print_hex(*((unsigned int*) registros[esp]+2),8,41,32,C_FG_WHITE | C_BG_LIGHT_GREY);
    print_hex(*((unsigned int*) registros[esp]+3),8,41,34,C_FG_WHITE | C_BG_LIGHT_GREY);
} 
