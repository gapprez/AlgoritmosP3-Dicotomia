#include <math.h>
#include <stdlib.h>
#include "dicotomia.h"


double execute(cota_t op, double n, int derivada){

	if (!derivada) {

		switch (op.tipo) {
			case 0:
				return pow(n,op.exp);
			case 1:
				return pow(n,op.exp)*log(n);
			default:
				return -1;
		}

	} else {

		switch (op.tipo) {
			case 0:
				if (op.exp == 0)
					return 0;
				else
					return op.exp*pow(n,op.exp-1);
			case 1:
				if (op.exp == 0)
					return 1/n;
				else
					return (op.exp*pow(n,op.exp-1)*log(n))+(pow(n,op.exp)/n);
			default:
				return -1;
		}
	}
}

/*void complejidad(void (*ord)(int [], int),
				 void (*ini)(int [], int), int cod_op) {

	double ta, tb, t, ti;
	int k, n, nf, mul;
	int* v;
	cotas c;
	c.subS = malloc(1024);
	c.sobreS = malloc(1024);
	c.ajusS = malloc(1024);

	obtenerN(cod_op, &c, &n, &nf, &mul);

	printf("%12s\t%12s\t%10s\t%10s\t%10s\n", "n", "t(n)", c.sobreS, c.ajusS, c.subS);
	for (n = n; n <= nf; n = n * mul) {
		v = malloc(sizeof(int)*n);
		ini(v, n);
		ta = microsegundos();
		ord(v, n);
		tb = microsegundos();
		t = tb - ta;
		calculoCotas(cod_op, n, &c);
		if (t < 500) {
			ta = microsegundos();
			for (k = 0; k < 1000; k++) {
				ini(v, n);
				ord(v, n);
			}
			tb = microsegundos();
			t = tb - ta;

			ta = microsegundos();
			for (k = 0; k < 1000; k++) {
				ini(v, n);
			}
			tb = microsegundos();

			ti = tb - ta;
			t = (t - ti) / k;
			printf("(*)%9d\t%12.3f\t%.8f\t%.8f\t%.8f\n", n, t, t/(c.subF), t/(c.ajusF), t/(c.sobreF));
		}
		else printf("%12d\t%12.3f\t%.8f\t%.8f\t%.8f\n", n, t, t/(c.subF), t/(c.ajusF), t/(c.sobreF));
		free(v);
	}

	free(c.subS);
	free(c.sobreS);
	free(c.ajusS);
}*/

/*Algoritmos de ordenación*/


void interCambioCotas(cota_t *cota1, cota_t *cota2){
	cota_t aux;

	aux = *cota1;
	*cota1 = *cota2;
	*cota2 = aux;
}

void ord_ins_dico (cota_t *v, int n, double punto) {
	int i, j;
	cota_t cotaAux;
	double valorCotaAux;

	for (i=1; i<n; i++) {
		cotaAux = v[i];
		valorCotaAux = execute(cotaAux, punto, 1);
		j = i-1;
		while (j>=0 && execute(v[j], punto, 1)>valorCotaAux) {
			interCambioCotas(&v[j+1],&v[j]);
			j--;
		}
		interCambioCotas(&v[j+1],&cotaAux);
	}
}

// TODO IMPLEMENTAR ORDENACION RAPIDA DE COTAS
void intercambiar_dico (cota_t* i, cota_t* j) {
	cota_t aux;
	aux = *i;
	*i = *j;
	*j = aux;
}

/*void Mediana3_dico(cota_t *v, int i, int j, int n) {
	int k;
	k = (i + j)/2;

	double valK = execute(v[k].cota , n, v[k].exp, 1);
	double valJ = execute(v[j].cota , n, v[j].exp, 1);
	double valI = execute(v[i].cota , n, v[i].exp, 1);

	if (valK > valJ) {
		intercambiar_dico(&v[k], &v[j]);
	}
	if (valK > valI) {
		intercambiar_dico(&v[k],&v[i]);
	}
	if (valI > valJ) {
		intercambiar_dico(&v[i], &v[j]);
	}
}

void OrdenarAux_dico(cota_t **v, int izq, int der, int n) {
	double pivote;
	int i, j;

	if ((izq + UMBRAL_DICO) <= der) {
		Mediana3_dico(v, izq, der, n);

		pivote = execute(v[izq].cota,n,v[izq].exp,1);
		i = izq;
		j = der;

		while (j>i) {
			i++;
			while (execute(v[i].cota,n,v[i].exp,1)< pivote) {
				i++;
			}
			j--;
			while (execute(v[j].cota,n,v[j].exp,1) > pivote) {
				j--;
			}
			intercambiar_dico(&v[i], &v[j]);
		}

		intercambiar_dico(&v[i], &v[j]);
		intercambiar_dico(&v[izq], &v[j]);
		OrdenarAux_dico(v, izq, j-1, n);
		OrdenarAux_dico(v, j+1, der, n);
	}
}

void ord_rapida_dico(cota_t **v, int n, int punto) {
	OrdenarAux_dico(v, 0, n-1, punto);
	if (UMBRAL_DICO > 1){
		ord_ins_dico(v, n, punto);
	}
}*/
//
void initFuncs(cota_t funcs[NUM_FUNCT]){

	printf(" ************************************ \n\n");
	printf(" - Definiendo Funciones \n");
	printf(" ************************************ \n");

	cota_t Nexp_x = {"n^$",0, 0};
	cota_t Nexp_x_LOG = {"n^$*log(n)",1, 0};

	funcs[0] = Nexp_x;
	funcs[1] = Nexp_x_LOG;
	printf("\n");

}


sit_dico initStudyCase(char *name, void (*ini)(int [], int)){
	sit_dico caso;
	strcpy(caso.sit_name, name);
	caso.func = ini;
	return caso;
}

tAlgoritmo initAlgorithem(char *name, void (*func)(int [], int), int ini, int mult,
                        int fin){
	int i;
	tAlgoritmo algoritmo;

	strcpy(algoritmo.alg_name, name);
	algoritmo.ini = ini;
	algoritmo.mult = mult;
	algoritmo.fin = fin;

	/*for(i = 0; i<NUM_SITUATIONS; i++)
		algoritmo.situation[i] = sitDico[i];*/

	return algoritmo;
}

void printAlgorithemSituation(tAlgoritmo *algoritmos){

	int i,j;

	for(i=0; i<NUM_ALGORITHEMS; i++){
		printf("\t - %s - tamaño vector de %d a %d de %d en %d\n", algoritmos[i].alg_name,algoritmos[i].ini,algoritmos[i].fin, algoritmos[i].mult,algoritmos[i].mult);
		/*for(j=0; j<NUM_SITUATIONS; j++){
			printf("\t\t * situacion %s \n",algoritmos[i].situation[j].sit_name);
		}*/
	}
	printf("\n");
}


void printFuncs(cota_t *funcs){
	int i;

	for(i = 0; i < NUM_FUNCT; i++){
		printf("\t - index %d - %s \n", funcs[i].tipo, funcs[i].name);
	}
	printf("\n");
}

/*
 * TODO - inicializa las cotas, para ello tiene que crearlas usando las funciones y habrá que ordenarlas
 */
void initCotas(cota_t funcs[], cota_t *cotas, int *nCotas) {
	int i;
	double j;
	char *token1;
	char *token2;
	char aux[256];

	printf(" - Inicializando Cotas\n");
	printf(" ************************************ \n");

	*nCotas = 0;

	for(i = 0; i<NUM_FUNCT; i++){

		j = 0;
		while(j < 3.2){
			cotas[*nCotas] = funcs[i];
			cotas[*nCotas].exp = j;
			token1 = strtok(cotas[*nCotas].name,"$");
			token2 = strtok(NULL,"$");
			if(token2 != NULL) {
				sprintf(aux, "%s%2.1f%s", token1, cotas[*nCotas].exp, token2);
				strcpy(cotas[*nCotas].name, aux);
			} else {
				sprintf(aux, "%s%2.1f", token1, cotas[*nCotas].exp);
				strcpy(cotas[*nCotas].name, aux);
			}
			if (i==1)
				j = j + 1;
			else
				j = j + 0.2;
			(*nCotas)++;
		}
	}
	printf("\n");
}

/*
 * TODO - las cotas tienen que estar ordenadas,
 * al ser crecientes y al no tener puntos de inflexión debería llegar con ver la derivada en el punto inicial
 * si una función tiene mas pendiente cerca del punto inicial tendría que estar siempre por encima
 */
void sortCotas(cota_t *cotas, int nCotas, double punto) {
	printf(" - Ordenando Cotas \n");
	printf(" ************************************ \n");
	ord_ins_dico(cotas,nCotas,punto);
	printf("\n");
}

int conseguirCotas (tAlgoritmo algoritmo, double tiempos[], cota_t* cotas, int inicio, int fin, cota_t cotasFinales[3]) {
	int i = 0, b = 1, medio, n, fallo = 0;
	double valoresn[NUM_TIEMPOS];
	cota_t cotaM;
	double divisionAct, divisionAnt;

	if (fin-inicio >= 1) {
		medio = (fin + inicio) / 2;
		cotaM = cotas[medio];

		for (n = algoritmo.ini; n <= algoritmo.fin; n = n * algoritmo.mult) {
			valoresn[i] = execute(cotaM, n, 0);
			i++;
		}

		i = 1;

		while (b && i < NUM_TIEMPOS) {
				divisionAnt = tiempos[i-1]/valoresn[i-1];
				divisionAct = tiempos[i]/valoresn[i];
				if ((divisionAct< divisionAnt-divisionAnt*UMBRAL_DICO) || (divisionAct>divisionAnt+divisionAnt*UMBRAL_DICO))
					if (fallo == MAX_FALLOS) {
						b = 0;
					}
					else {
						fallo++;
						i++;
					}
				else
					i++;
		}

		if (!b) {
			if (fin - inicio != 1) {
				if ((divisionAct< divisionAnt-divisionAnt*UMBRAL_DICO))
					conseguirCotas(algoritmo, tiempos, cotas, inicio, medio - 1, cotasFinales);
				else
					conseguirCotas(algoritmo, tiempos, cotas, medio + 1, fin, cotasFinales);
			}
			else {
				return 0;
			}
		}
		else {
			cotasFinales[0] = cotas[medio - 1];
			cotasFinales[1] = cotas[medio];
			cotasFinales[2] = cotas[medio + 1];
			return 1;
		}
	}
	else {
		return 0;
	}
}
void printCotas(cota_t *cotas, int nCotas){
	int i = 0;

	for(i = 0; i < nCotas; i++){
		printf("\t - Cota %d función %s, valor: %f, pendiente: %f\n",i,cotas[i].name, execute(cotas[i], pow(10,8), 0), execute(cotas[i], pow(10,8),  1));
	}

	printf("\n");
}