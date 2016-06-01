#include "pila.h"

void nueva_pila(pila *p,unsigned int* base){
	p->base = base;
	p->tope = base;
	p->cant = 0;
}

unsigned int pop_pila(pila* p){
	p->cant-=1;
	p->tope+=1;
	return *(p->tope-1);
}

void push_pila(pila* p,unsigned int push){
	p->cant+=1;
	p->tope-=1;
	*(p->tope)=push;
}