/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"
#include "screen.h"

id _proximo_tipo(id _actual){
	switch (_actual){
		case A:
			return B;
		case B:
			return H;
		case H:
			return A;
	}

	return 0;
}

unsigned short _tiene_vivos(id _id){
	unsigned short _algun_vivo = 0;
	if (_id == H) {
		int i = 0;
		while(i < CANT_H && _algun_vivo == 0) {
			_algun_vivo = GAME.iniciales[i].vivo;
			i++;
		}
	} else {
		int i = 0;
		while(i < CANT_TAREAS_J && _algun_vivo == 0){
			_algun_vivo = GAME.js[_id].tareas[i].vivo;
			i++;
		}
	}

	return _algun_vivo;
}

void _proximaSana(){
	do{
		GAME.actual++;
		if(GAME.actual >= CANT_H) GAME.actual = 0;
	}while(GAME.iniciales[GAME.actual].vivo == 0);
}

void _proximaJug(id _id){
	jugador* j = &(GAME.js[_id]);
	do{
		j->actual++;
		if(j->actual >= CANT_TAREAS_J) j->actual = 0;
	}while(j->tareas[j->actual].vivo == 0);
}

unsigned char prox_reloz(unsigned char r){
	unsigned char re = r;
	if(r=='-'){
	 re = '\\';
	}
	else if(r=='\\'){
		re = '|';
	}
	else if(r=='|'){
			re = '/';
	}
	else if(r=='/'){
			re = '-';
	}

return re;
}

unsigned short _selector_proxima_tarea(id _id){
	unsigned short _siguiente_selector;
	if (_id == H) {
		unsigned int _actual_selector = GAME.tareaActual->selector_tss;
		_proximaSana();
		GAME.tareaActual = &GAME.iniciales[GAME.actual];
		_siguiente_selector = GAME.tareaActual->selector_tss;

		if(_siguiente_selector == _actual_selector){
			_siguiente_selector = 0;
		}
	} else {
		//print_int(GAME.js[_id].actual,50,0,C_FG_WHITE | C_BG_CYAN);
		_proximaJug(_id);
		
		unsigned int _actual_selector = GAME.tareaActual->selector_tss;
		unsigned int actual = GAME.js[_id].actual;
		

		GAME.tareaActual = &GAME.js[_id].tareas[actual];
		_siguiente_selector = GAME.tareaActual->selector_tss;
		
		if(_siguiente_selector == _actual_selector){
			_siguiente_selector = 0;
		}
	}
	GAME.tareaActual->reloz = prox_reloz(GAME.tareaActual->reloz);
	actualizar_display_relozJ();
	actualizar_display_relozS();
	return _siguiente_selector;
}

unsigned short sched_proximo_indice() {

	id _actual = GAME.corriendo;

	id _prox = _proximo_tipo(_actual);
	if (_tiene_vivos(_prox)){
		GAME.corriendo = _prox;
		return _selector_proxima_tarea(_prox);
	} 

	id _prox_prox = _proximo_tipo(_prox);
	if (_tiene_vivos(_prox_prox)) {
		GAME.corriendo = _prox_prox;
		return _selector_proxima_tarea(_prox_prox);
	}

	if (_tiene_vivos(_actual)){
		GAME.corriendo = _actual;
		unsigned short _proxSegmento = _selector_proxima_tarea(_actual);
		return _proxSegmento;
	}

	return 0;
}
