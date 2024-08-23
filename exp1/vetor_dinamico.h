#ifndef VETOR_DINAMICO_H
#define VETOR_DINAMICO_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
/**
 * Vetor dinamico genérico
 */
#define DECL_VETOR_DINAMICO(nome_vetor, tipo)\
typedef struct nome_vetor {\
	tipo *buf;\
	int tam, cap;\
} nome_vetor;\
\
nome_vetor nome_vetor##_novo(void);\
\
bool nome_vetor##_set_cap(nome_vetor *v, int nova_cap);\
\
bool nome_vetor##_append(nome_vetor *v, tipo x);\
\
bool nome_vetor##_insert(nome_vetor *v, tipo x, int i);\
\
bool nome_vetor##_pop(nome_vetor *v);\
\
bool nome_vetor##_remove(nome_vetor *v, int i);

#define GEN_VETOR_DINAMICO(nome_vetor, tipo)\
nome_vetor nome_vetor##_novo(void) { return (nome_vetor){.buf = malloc(8 * sizeof(tipo)), .cap = 8, .tam = 0}; }\
\
bool nome_vetor##_set_cap(nome_vetor *v, int nova_cap) {\
	tipo *buf = realloc(v->buf, nova_cap * sizeof(tipo));\
	if (buf != NULL) {\
		v->buf = buf;\
		v->cap = nova_cap;\
		return true;\
	}\
	return false;\
}\
\
bool nome_vetor##_append(nome_vetor *v, tipo x) {\
	if (v->tam == v->cap) {\
		bool mudou = nome_vetor##_set_cap(v, v->cap * 2);\
		if (!mudou) return false;\
	}\
	v->buf[v->tam++] = x;\
	return true;\
}\
\
bool nome_vetor##_insert(nome_vetor *v, tipo x, int i) {\
	if (i < 0) return false;\
	if (i > v->tam) return false;\
	if (i == v->tam) return nome_vetor##_append(v, x);\
	if (v->tam == v->cap) {\
		bool mudou = nome_vetor##_set_cap(v, v->cap * 2);\
		if (!mudou) return false;\
	}\
	for (int j = v->tam - 1; j >= i; j--) {\
		v->buf[j + 1] = v->buf[j];\
	}\
	v->buf[i] = x;\
	v->tam++;\
	return true;\
}\
\
bool nome_vetor##_pop(nome_vetor *v) {\
	if (v->tam == 0) return false;\
	if (v->tam > 8 && v->tam <= v->cap / 4) {\
		nome_vetor##_set_cap(v, v->cap / 2);\
	}\
	v->tam--;\
	return true;\
}\
\
bool nome_vetor##_remove(nome_vetor *v, int i) {\
	if (i < 0 || i >= v->tam) return false;\
	for (; i < v->tam - 1; i++) {\
		v->buf[i] = v->buf[i + 1];\
	}\
	v->tam--;\
	return true;\
}

#endif
