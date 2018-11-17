#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>

#include "dicotomia.h"

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define LONGITUD_CLAVE 30
#define LONGITUD_SINONIMOS 300
#define MAX_N 38197

#define UMBRAL 10


typedef int pos;

typedef struct entrada_ {
	int ocupada;
	char clave [LONGITUD_CLAVE];
	char sinonimos [LONGITUD_SINONIMOS];
} entrada;

typedef entrada *tabla_cerrada;

typedef struct {
	char clave [LONGITUD_CLAVE];
	char sinonimos [LONGITUD_SINONIMOS];
} item;


int dispersionA(char *clave, int tamTabla) {
	int i, valor = clave[0], n = MIN(8, strlen(clave));
	for (i = 1; i < n; i++)
		valor += clave[i];

	return valor % tamTabla;
}

int dispersionB(char *clave, int tamTabla) {
	int i, n = MIN(8, strlen(clave));
	unsigned int valor = clave[0];
	for (i = 1; i < n; i++)
		valor = (valor<<5) + clave[i]; /* el desplazamiento de 5 bits equivale a */
	return valor % tamTabla; /* multipicar por 32 */
}

unsigned int resolDoble(int pos_ini, int num_intento) {
	return (unsigned int) 10007 - pos_ini % 10007;
}

unsigned int resolN(int pos_ini, int num_intento) {
	return (unsigned int) num_intento;
}

unsigned int resolCuadratica(int pos_ini, int num_intento) {
	return (unsigned int) pow(num_intento, 2);
}

int ndispersion(char *clave, int tamTabla) {
	if (strcmp(clave, "ANA")==0) return 7;
	if (strcmp(clave, "JOSE")==0) return 7;
	if (strcmp(clave, "OLGA")==0) return 7;
	return 6;
}

int leer_sinonimos(item datos[]) {
	char c;
	int i, j;
	FILE *archivo;
	if ((archivo = fopen("sinonimos.txt", "r")) == NULL) {
		printf("Error al abrir ’sinonimos.txt’\n");
		return(EXIT_FAILURE);
	}
	for (i = 0; fscanf(archivo, "%s", datos[i].clave) != EOF; i++) {
		if ((c = fgetc(archivo)) != '\t') {
			printf("Error al leer el tabulador\n");
			return(EXIT_FAILURE);
		}
		for (j = 0; (c = fgetc(archivo)) != '\n'; j++) {
			if (j < LONGITUD_SINONIMOS - 1)
				datos[i].sinonimos[j] = c;
		}
		datos[i].sinonimos[MIN(j, LONGITUD_SINONIMOS -1)] = '\0';
	}
	if (fclose(archivo) != 0) {
		printf("Error al cerrar el fichero\n");
		return(EXIT_FAILURE);
	}
	return(i);
}

void inicializar_cerrada(tabla_cerrada *diccionario, int tam) {
	int i;

	for (i = 0; i<tam; i++) {
		(*diccionario)[i].ocupada = 0;
		strcpy((*diccionario)[i].clave,"");
		strcpy((*diccionario)[i].sinonimos,"");
	}
}

pos buscar_cerrada(char *clave, tabla_cerrada diccionario, int tam,
                   int *colisiones, int (*dispersion)(char *, int),
                   unsigned int (*resol_colision)(int pos_ini, int num_intento)){
	int x = dispersion(clave, tam);
	int posActual = x;

	while ((diccionario[posActual].ocupada) && (strcmp(diccionario[posActual].clave,clave))) {
		*colisiones = *colisiones + 1;
		posActual = (x + resol_colision(x, *colisiones)) % tam;
	}


	return posActual;
}

int insertar_cerrada(char *clave, char *sinonimos,
                     tabla_cerrada *diccionario, int tam,
                     int (*dispersion)(char *, int),
                     unsigned int (*resol_colision)(int pos_ini, int num_intento)) {
	int colisiones = 0;
	pos pos = buscar_cerrada(clave, *diccionario, tam, &colisiones, dispersion, resol_colision);

	if ((*diccionario)[pos].ocupada == 0) {
		(*diccionario)[pos].ocupada = 1;
		strcpy((*diccionario)[pos].clave,clave);
		strcpy((*diccionario)[pos].sinonimos, sinonimos);
		return colisiones;
	}
	else
		printf("%d, %d", (*diccionario)[pos].ocupada, pos);

	return -1;

}

void mostrar_cerrada(tabla_cerrada diccionario, int tam){
	int i;

	printf("{\n");
	for(i = 0; i<tam; i++){
		printf("%d- %s\n",i,diccionario[i].clave);
	}
	printf("}\n");

}

/* obtiene la hora actual en microsegundos */
double microsegundos(){
	struct timeval t;
	if (gettimeofday(&t, NULL) < 0)
		return 0.0;
	return (t.tv_usec + t.tv_sec * 1000000.0);
}/* obtiene la hora actual en microsegundos */



/* INICIALIZACIÖN DEL VECTOR */

void inicializar_semilla() {
	srand(time(NULL));
}

void aleatorio(int v [], int n) {/* se generan números pseudoaleatorio entre -n y +n */
	int i, m=2*n+1;
	for (i=0; i < n; i++)
		v[i] = (rand() % m) - n;
}

void ascendente(int v [], int n) {
	int i;
	for (i=0; i < n; i++)
		v[i] = i;
}


void descendente (int v[], int n){
	int i;

	for (i = 0; i < n; i++) {
		v[i] = n - i - 1;
	}
}

void printDerivInPoint(cota_t *cotas, int nCotas, int point){

	int i;

	for(i=0; i<nCotas; i++){
		printf(" %f,", execute(cotas[i],point, 1));
	}
	printf("\n");
}

// EJEMPLO PARA EL ALGORITMO DE DICOTOMIA
/*Algoritmos de ordenación*/

void ord_ins (int v [], int n) {
	int i, j, x;
	for (i=1; i<n; i++) {
		x = v[i];
		j = i-1;
		while (j>=0 && v[j]>x) {
			v[j+1] = v[j];;
			j--;
		}
		v[j+1] = x;
	}
}

void intercambiar (int* i, int* j) {
	int aux;
	aux = *i;
	*i = *j;
	*j = aux;
}

void Mediana3(int v[], int i, int j) {
	int k;
	k = (i + j)/2;

	if (v[k] > v[j]) {
		intercambiar(&v[k], &v[j]);
	}
	if (v[k] > v[i]) {
		intercambiar(&v[k],&v[i]);
	}
	if (v[i] > v[j]) {
		intercambiar(&v[i], &v[j]);
	}
}

void OrdenarAux(int v[], int izq, int der) {
	int pivote, i, j;

	if ((izq + UMBRAL) <= der) {
		Mediana3(v, izq, der);

		pivote = v[izq];
		i = izq;
		j = der;

		while (j>i) {
			i++;
			while (v[i] < pivote) {
				i++;
			}
			j--;
			while (v[j] > pivote) {
				j--;
			}
			intercambiar(&v[i], &v[j]);
		}

		intercambiar(&v[i], &v[j]);
		intercambiar(&v[izq], &v[j]);
		OrdenarAux(v, izq, j-1);
		OrdenarAux(v, j+1, der);
	}
}

void ord_rapida(int v[], int n) {
	OrdenarAux(v, 0, n-1);
	if (UMBRAL > 1){
		ord_ins(v, n);
	}
}

void leerTiempos(const tAlgoritmo algoritmo, double tiempos[]) {
	int n, i = 0, k;
	int *v;
	double ta, tb, ti, t;
	int inicio = algoritmo.ini;
	int fin = algoritmo.fin;
	int mult = algoritmo.mult;

	for (n = inicio; n <= fin; n = n * mult) {
		v = malloc(sizeof(int) * n);
		ascendente(v, n);
		ta = microsegundos();
		algoritmo.func(v, n);
		tb = microsegundos();
		t = tb - ta;

		if (t < 500) {
			ta = microsegundos();
			for (k = 0; k < 1000; k++) {
				ascendente(v, n);
				algoritmo.func(v, n);
			}
			tb = microsegundos();
			t = tb - ta;

			ta = microsegundos();
			for (k = 0; k < 1000; k++) {
				ascendente(v, n);
			}
			tb = microsegundos();

			ti = tb - ta;
			t = (t - ti) / k;

		}
		tiempos[i] = t;
		i++;
		free(v);
	}
}

void test(unsigned int (*resol_colision)(int, int)) {
	int tam = 11;
	int colTotal = 0;
	int i = 0;
	pos pos;
	tabla_cerrada diccionario = malloc(sizeof(entrada)*tam);
	char* nombres[7] = {"ANA", "LUIS", "JOSE", "OLGA", "ROSA", "IVAN", "CARLOS"};

	inicializar_cerrada(&diccionario, tam);


	for (i = 0; i < 6; i++) {
		colTotal += insertar_cerrada(nombres[i], "", &diccionario, tam, ndispersion, resol_colision);
	}
	mostrar_cerrada(diccionario, tam);
	printf("Numero total de colisiones al insertar los elementos: %d\n\n", colTotal);
	colTotal = 0;

	for (i = 0; i < 7; i++) {
		pos = buscar_cerrada(nombres[i], diccionario, tam, &colTotal, ndispersion, resol_colision);
		if (diccionario[pos].ocupada)
			printf("Al buscar: %s, encuentro: %s, colisiones: %d\n", nombres[i], diccionario[pos].clave, colTotal);
		else
			printf("No encuentro: %s, colisiones: %d\n", nombres[i], colTotal);
		colTotal = 0;
	}

	printf("\n");
	free(diccionario);
}

int main() {

	/*int nCotas = 0;
	int fallado = 0;

	double tiempos[NUM_TIEMPOS];
	cota_t funcs[NUM_FUNCT];
	cota_t *cotasEstudio;
	cota_t cotasFinales[3];
	cotasEstudio = malloc(sizeof(cota_t)*100);
	tAlgoritmo insercion;
	insercion.ini = 500; insercion.ini = 1000;
	insercion.fin = 32000; insercion.fin = (int) pow(10, 8);
	insercion.mult = 2; insercion.mult = 10;
	insercion.func = ord_ins; insercion.func = ord_rapida;
	strcpy(insercion.alg_name,"Insercion");
	inicializar_semilla();

	// Inicializamos el muestrario de funciones y los algoritmos de estudio
	initFuncs(funcs);
	printFuncs(funcs);
	//initAlgorithems(algoritmos);
	//printAlgorithemSituation(algoritmos);
	// Generamos las Posibles cotas
	initCotas(funcs, cotasEstudio, &nCotas);
	printCotas(cotasEstudio, nCotas);

	// Ordenamos las Posibles Cotas basándonos en la pendiente de las funciones en un punto
	// representativo del intervalo en el que se realiza el estudio
	// printDerivInPoint(cotasEstudio, nCotas, 20000);
	sortCotas(cotasEstudio, nCotas, pow(10, 8));
	printCotas(cotasEstudio, nCotas);

	leerTiempos(insercion, tiempos);
	insercion.ini = 500; insercion.ini = 1000;
	insercion.fin = 32000; insercion.fin = (int) pow(10, 8);
	insercion.mult = 2; insercion.mult = 10;
	insercion.func = ord_ins; //insercion.func = ord_rapida;
	leerTiempos(insercion, tiempos);
	insercion.ini = 500; insercion.ini = 1000;
	insercion.fin = 32000; insercion.fin = (int) pow(10, 8);
	insercion.mult = 2; insercion.mult = 10;
	insercion.func = ord_ins; //insercion.func = ord_rapida;

	while (!conseguirCotas(insercion, tiempos, cotasEstudio, 0, nCotas-1, cotasFinales)) {
		leerTiempos(insercion, tiempos);
		insercion.ini = 500; insercion.ini = 1000;
		insercion.fin = 32000; insercion.fin = (int) pow(10, 8);
		insercion.mult = 2; insercion.mult = 10;
		insercion.func = ord_ins; //insercion.func = ord_rapida;
		fallado++;
	}

	printf("El calculo ha fallado %d veces\n", fallado);
	printf("Las cotas finales son:\n");
	printf("cota subestimada -> %s\n", cotasFinales[0].name);
	printf("cota ajustada -> %s\n", cotasFinales[1].name);
	printf("cota subestimada -> %s\n", cotasFinales[2].name);
	//medirTiempos();
	//acorarComplejidad();

	//mostrarResultados();

	//tabla_cerrada d = malloc (MAX_N * sizeof(entrada));


	free(cotasEstudio);
	return 0;*/
	tabla_cerrada diccionario;
	test(resolN);
	test(resolCuadratica);
	test(resolDoble);

	/*diccionario = malloc(MAX_N*sizeof(entrada));
	inicializar_cerrada(&diccionario, MAX_N);
	leer_sinonimos(&diccionario);*/

	//free(diccionario);
}