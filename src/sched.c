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
		while(i < 15 && _algun_vivo == 0) {
			_algun_vivo = GAME.iniciales[i].vivo;
			i++;
		}
	} else {
		int i = 0;
		while(i < 5 && _algun_vivo == 0){
			_algun_vivo = GAME.js[_id].tareas[i].vivo;
			i++;
		}
	}

	return _algun_vivo;
}

void _proximaSana(){
	do{
		GAME.proximaSana ++;
		if(GAME.proximaSana >= 15) GAME.proximaSana = 0;
	}while(GAME.iniciales[GAME.proximaSana].vivo == 0);
}

void _proximaJug(id _id){
	jugador* j = &(GAME.js[_id]);
	do{
		j->proxima ++;
		if(j->proxima >= 15) j->proxima = 0;
	}while(j->tareas[j->proxima].vivo == 0);
}

unsigned short _selector_proxima_tarea(id _id){
	unsigned short _siguiente_selector;
	
	if (_id == H) {
		unsigned int prox = GAME.proximaSana;
		_siguiente_selector = GAME.iniciales[GAME.proximaSana].selector_tss;
		_proximaSana();

		if (prox == GAME.proximaSana){
			_siguiente_selector = 0;
		}

	} else {
		unsigned int prox = GAME.js[_id].proxima;
		_siguiente_selector = GAME.js[_id].tareas[prox].selector_tss;
		_proximaJug(_id);

		if (prox == GAME.js[_id].proxima){
			_siguiente_selector = 0;
		}
	}
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