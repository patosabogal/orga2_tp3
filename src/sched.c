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

unsigned int _no_tiene_vivos(id _id){
	return FALSE;
}


unsigned short sched_proximo_indice() {
	id _actual = GAME.corriendo;

	id _prox = _proximo_tipo(_actual);
	while (_no_tiene_vivos(_prox) && _prox != _actual ) {
		_prox = _proximo_tipo(_prox);
	}

	if (_prox == _actual && _no_tiene_vivos(_actual)){
		// todos muertos
		return 0;
	} else {
		// no estan todos muertos
		// ACA TE DEVUELVO EL SELECTOR DE LA TAREA
		return 0;
	}

	return 0;	
}
