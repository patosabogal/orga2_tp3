#ifndef __PILA_H__
#define __PILA_H__

typedef struct str_pila{
	unsigned int* base;
	unsigned int* tope;
	unsigned int cant;
} pila;

void nueva_pila(pila* p, unsigned int* base);
void push_pila(pila* p,unsigned int push);
unsigned int pop_pila(pila* p);

#endif  /* !__PILA_H__ */