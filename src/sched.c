/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"

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
		while(i < 15 && !_algun_vivo) _algun_vivo = GAME.iniciales[i].vivo;
	} else {
		int i = 0;
		while(i < 5 && !_algun_vivo) _algun_vivo = GAME.js[_id].tareas[i].vivo;
	}

	return _algun_vivo;
}

unsigned short _selector_proxima_tarea(id _id){
	// COMO ACCEDO AL SELECTOR DE UNA TAREA
	return 0;
}

unsigned short sched_proximo_indice() {
	id _actual = GAME.corriendo;

	id _prox = _proximo_tipo(_actual);
	if (_tiene_vivos(_prox)) return _selector_proxima_tarea(_prox);

	id _prox_prox = _proximo_tipo(_prox);
	if (_tiene_vivos(_prox_prox)) return _selector_proxima_tarea(_prox_prox);

	if (_tiene_vivos(_actual)) return _selector_proxima_tarea(_actual);

	return 0;
}
