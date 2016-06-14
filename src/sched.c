/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"
#include "screen.h"

scheduler SCHED;

void sched_inicializar(){
	SCHED.actual = 0;
	SCHED.corriendo = H;
	SCHED.js[A].actual = 0;
	SCHED.js[B].actual = 0;

	SCHED.idle = TRUE;
	sched_inicializar_tareas_iniciales();
	sched_inicializar_tareas_jugadores();
}

void sched_inicializar_tareas_iniciales(){
	int i;
	for (i = 0; i < CANT_H; ++i)
	{
		SCHED.tareas[i].vivo = FALSE;
		//SCHED.tareas[i] = sched_nueva_tarea((unsigned int *)CODIGO_TAREA_H, 16+i, 16-i,H);
	}
}

void sched_inicializar_tareas_jugadores(){
	int i;
	for (i = 0; i < CANT_TAREAS_J; ++i)
	{
		SCHED.js[A].tareas[i].vivo = FALSE;
		SCHED.js[B].tareas[i].vivo = FALSE;
	}
}

tarea sched_nueva_tarea(unsigned int* codigo, unsigned int x, unsigned int y, id tipo){
	unsigned short _selector_tss = 0;
	unsigned int _cr3 = 0;
	tss_nueva(codigo, x, y, &_selector_tss, &_cr3);
	tarea nueva_tarea;
	nueva_tarea.x = x;
	nueva_tarea.y = y;
	nueva_tarea.x_map = 0;
	nueva_tarea.y_map = 0;
	nueva_tarea.map = FALSE;

	nueva_tarea.vivo = TRUE;
	nueva_tarea.virus = tipo;
	nueva_tarea.selector_tss = _selector_tss;
	nueva_tarea.cr3 = _cr3;
	nueva_tarea.reloj = '-';
	return nueva_tarea;
}


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
			_algun_vivo = SCHED.tareas[i].vivo;
			i++;
		}
	} else {
		int i = 0;
		while(i < CANT_TAREAS_J && _algun_vivo == 0){
			_algun_vivo = SCHED.js[_id].tareas[i].vivo;
			i++;
		}
	}

	return _algun_vivo;
}

void _proximaSana(){
	do{
		SCHED.actual++;
		if(SCHED.actual >= CANT_H) SCHED.actual = 0;
	}while(SCHED.tareas[SCHED.actual].vivo == 0);
}

void _proximaJug(id _id){
	schedJ* j = &(SCHED.js[_id]);
	do{
		j->actual++;
		if(j->actual >= CANT_TAREAS_J) j->actual = 0;
	}while(j->tareas[j->actual].vivo == 0);
}

unsigned char prox_reloj(unsigned char r){
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
	unsigned int _actual_selector;

	if(SCHED.idle){
		_actual_selector = 0;
	}else{
		_actual_selector = SCHED.tareaActual->selector_tss;
		SCHED.idle = FALSE;
	}

	if (_id == H) {
		_proximaSana();
		SCHED.tareaActual = &SCHED.tareas[SCHED.actual];
		_siguiente_selector = SCHED.tareaActual->selector_tss;

		if(_siguiente_selector == _actual_selector){
			_siguiente_selector = 0;
		}
	} else {
		
		_proximaJug(_id);
		unsigned int actual = SCHED.js[_id].actual;
		SCHED.tareaActual = &SCHED.js[_id].tareas[actual];
		_siguiente_selector = SCHED.tareaActual->selector_tss;

		if(_siguiente_selector == _actual_selector){
			_siguiente_selector = 0;
		}
	}
	SCHED.tareaActual->reloj = prox_reloj(SCHED.tareaActual->reloj);
	
	actualizar_display_relojJ();
	actualizar_display_relojS();

	return _siguiente_selector;
}

unsigned short sched_proximo_indice() {
	if(GAME.HALT){
		return 0;
	}
	
	id _actual = SCHED.corriendo;

	id _prox = _proximo_tipo(_actual);
	if (_tiene_vivos(_prox)){
		SCHED.corriendo = _prox;
		return _selector_proxima_tarea(_prox);
	} 

	id _prox_prox = _proximo_tipo(_prox);
	if (_tiene_vivos(_prox_prox)) {
		SCHED.corriendo = _prox_prox;
		return _selector_proxima_tarea(_prox_prox);
	}

	if (_tiene_vivos(_actual)){
		SCHED.corriendo = _actual;
		unsigned short _proxSegmento = _selector_proxima_tarea(_actual);
		
		return _proxSegmento;
	}

	if(SCHED.idle){
		return 0;
	}

	SCHED.idle = TRUE; //Nada tiene nada vivo, vuelvo a la idle
	return 0x50;
}
