#include <stdio.h>
#include <string.h>

// Constantes Algoritmo Dicotomia
#define UMBRAL_DICO 0.05
#define NUM_SITUATIONS 3

#define NUM_ALGORITHEMS 2
#define NUM_FUNCT 2
#ifndef NUM_TIEMPOS
#define NUM_TIEMPOS 6
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
	cota_t sobre;
	cota_t ajus;
	cota_t sub;
	int tamV;
	void (*func)(int v[], int nargs);
	char sit_name[256];
} sit_dico;

typedef struct {
	int ini;
	int fin;
	int mult;
	void (*func)(int v[], int n);
	char alg_name[256];
} tAlgoritmo;

/* FIN REGISTROS DICOTOMIA  */

double execute(cota_t op , double n, int derivada);

void initFuncs(cota_t funcs[]);
void printFuncs(cota_t *funcs);
void initCotas(cota_t funcs[], cota_t *cotas, int *nCotas);
void sortCotas(cota_t *cotas, int nCotas, double punto);
void printCotas(cota_t *cotas, int nCotas);
int conseguirCotas (tAlgoritmo algoritmo, double tiempos[], cota_t* cotas, int inicio, int fin, cota_t cotasFinales[3]);

tAlgoritmo initAlgorithems(tAlgoritmo algoritmos[NUM_ALGORITHEMS]);
sit_dico initStudyCase(char *name, void (*ini)(int [], int));
tAlgoritmo initAlgorithem(char *name, void (*func)(int [], int), int ini, int mult,
                        int fin);
void printAlgorithemSituation(tAlgoritmo *algoritmos);

void acotarComplejidad(tAlgoritmo *algoritmos, cota_t *cotas);