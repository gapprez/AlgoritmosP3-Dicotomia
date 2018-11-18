#include <stdio.h>
#include <string.h>

// Constantes Algoritmo Dicotomia
#define UMBRAL_DICO 0.06
#define NUM_SITUATIONS 3

#define NUM_ALGORITHEMS 2
#define NUM_FUNCT 2
#ifndef NUM_TIEMPOS
#define NUM_TIEMPOS 8
#endif
#ifndef MAX_FALLOS
#define MAX_FALLOS (int) 2
#endif

 /* Registros DICOTOMIA */



typedef  struct {
	char name[256];
	int tipo;
	double exp;
}cota_t;

typedef struct {
	char *nombre;
	int (*func)(char*, int);
	int ini;
	int fin;
	int mult;
} tAlgoritmo;

/* FIN REGISTROS DICOTOMIA  */

double execute(cota_t op , double n, int derivada);
void initFuncs(cota_t funcs[]);
void printFuncs(cota_t *funcs);
void initCotas(cota_t funcs[], cota_t *cotas, int *nCotas);
void sortCotas(cota_t *cotas, int nCotas);
void printCotas(cota_t *cotas, int nCotas);
int conseguirCotas (tAlgoritmo algoritmo, double tiempos[], cota_t* cotas, int inicio, int fin, cota_t cotasFinales[3]);