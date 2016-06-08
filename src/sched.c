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

tarea _proxima_tarea_viva_H(unsigned short indice){
	tarea _tarea = GAME.iniciales[indice];

	do {
		indice++;
		if (indice == 15) indice = 0;
		_tarea = GAME.iniciales[indice];
	} while(_tarea.vivo == 0);

	return _tarea;
}

tarea _proxima_tarea_viva_jug(id _id){
	unsigned short indice = GAME.js[_id].actual;
	tarea _tarea = GAME.js[_id].tareas[indice];

	do {
		indice++;
		if (indice == 5) indice = 0;
		_tarea = GAME.js[_id].tareas[indice];
	} while(_tarea.vivo == 0);

	return _tarea;	
}

unsigned short _selector_proxima_tarea(id _id){
	unsigned short _siguiente_selector;
	if (_id == H) {
		_siguiente_selector = _proxima_tarea_viva_H(GAME.actualSana).selector_tss;
	} else {
		_siguiente_selector = _proxima_tarea_viva_jug(_id).selector_tss;
	}
	return _siguiente_selector;
}

unsigned short sched_proximo_indice() {
	id _actual = GAME.corriendo;

	id _prox = _proximo_tipo(_actual);

	if (_tiene_vivos(_prox)) return _selector_proxima_tarea(_prox);

	id _prox_prox = _proximo_tipo(_prox);
	if (_tiene_vivos(_prox_prox)) {
		return _selector_proxima_tarea(_prox_prox);
	}

	if (_tiene_vivos(_actual)) return _selector_proxima_tarea(_actual);

	return 0;
}