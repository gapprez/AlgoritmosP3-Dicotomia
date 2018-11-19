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
		while(j < EXP_MAX+0.1){
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
void sortCotas(cota_t *cotas, int nCotas) {
	double punto = pow(10, 8);

	printf(" - Ordenando Cotas \n");
	printf(" ************************************ \n");
	ord_ins_dico(cotas,nCotas,punto);
	printf("\n");
}

double calcularMedia(const double valores[NUM_TIEMPOS]) {
	int i;
	double total = 0;

	for (i = 0; i < NUM_TIEMPOS; i++) {
		total += valores[i];
	}

	return total/NUM_TIEMPOS;
}

double umbral_total = UMBRAL_MIN;
/*
 * Devuelve 1 si encuentra las cotas
 * Devuelve 0 si no encuentra las cotas con umbral_total < UMBRAL_MAX
 * Devuelve -1 si no encuentra las cotas con umbral_total = UMBRAL_MAX
 */
int conseguirCotas (tAlgoritmo algoritmo, double tiempos[], cota_t* cotas, int inicio, int fin, cota_t cotasFinales[3]) {
	int i = 0, medio, n, fallo = 0;
	double valoresn[NUM_TIEMPOS];
	cota_t cotaM;
	double divisionAct, media;
	double divisionPri, divisionUlt;

	if (fin >= inicio) {
		medio = (fin + inicio) / 2;
		cotaM = cotas[medio];

		for (n = algoritmo.ini; n <= algoritmo.fin; n = n * algoritmo.mult) {
			valoresn[i] = tiempos[i]/execute(cotaM, n, 0);
			i++;
		}

		i = 0;
		media = calcularMedia(valoresn);
		while (i < NUM_TIEMPOS) {
			divisionAct = valoresn[i];
			if ((divisionAct< media-media*umbral_total) || (divisionAct>media+media*umbral_total))
				fallo++;
			i++;
		}

		divisionPri = valoresn[0];
		divisionUlt = valoresn[NUM_TIEMPOS-1];
		if (fallo > MAX_FALLOS) { //Segunda oportunidad
			if (divisionUlt< divisionPri-divisionPri*umbral_total)
				return conseguirCotas(algoritmo, tiempos, cotas, inicio, medio - 1, cotasFinales);
			else if (divisionUlt > divisionPri + divisionPri*umbral_total)
				return conseguirCotas(algoritmo, tiempos, cotas, medio + 1, fin, cotasFinales);
			else { //Está dentro del rango
				if (fallo >= NUM_TIEMPOS-NUM_TIEMPOS*0.5) { //Se desestima cota debido a la cantidad de fallos
					divisionUlt = valoresn[NUM_TIEMPOS-2];
					divisionPri = valoresn[1];
					if (divisionUlt < divisionPri-divisionPri*umbral_total)
						return conseguirCotas(algoritmo, tiempos, cotas, inicio, medio - 1, cotasFinales);
					else if (divisionUlt > divisionPri+divisionPri*umbral_total)
						return conseguirCotas(algoritmo, tiempos, cotas, medio + 1, fin, cotasFinales);
					else {
						if (umbral_total < UMBRAL_MAX-0.005) {
							umbral_total = umbral_total + 0.01;
							return 0;
						}
						else {
							umbral_total = UMBRAL_MIN;
							return -1;
						}
					}
				}
				else {
					cotasFinales[0] = cotas[medio - 1];
					cotasFinales[1] = cotas[medio];
					cotasFinales[2] = cotas[medio + 1];
					printf("Se consiguió con umbral = %f\n", umbral_total);
					umbral_total = UMBRAL_MIN;
					return 1;
				}
			}
		}
		else if (fallo == MAX_FALLOS) { //Si tiene MAX_FALLOS, comprobación final con un umbral mayor
			if (divisionUlt< divisionPri-divisionPri*(umbral_total + umbral_total*1.5))
				return conseguirCotas(algoritmo, tiempos, cotas, inicio, medio - 1, cotasFinales);
			else if (divisionUlt > divisionPri + divisionPri*(umbral_total + umbral_total*1.5))
				return conseguirCotas(algoritmo, tiempos, cotas, medio + 1, fin-1, cotasFinales);
			else {
				cotasFinales[0] = cotas[medio - 1];
				cotasFinales[1] = cotas[medio];
				cotasFinales[2] = cotas[medio + 1];
				printf("Se consiguió con umbral = %f\n", umbral_total);
				umbral_total = UMBRAL_MIN;
				return 1;
			}
		}
		else {
			cotasFinales[0] = cotas[medio - 1];
			cotasFinales[1] = cotas[medio];
			cotasFinales[2] = cotas[medio + 1];
			printf("Se consiguió con umbral = %f\n", umbral_total);
			umbral_total = UMBRAL_MIN;
			return 1;
		}
	}
	else {
		if (umbral_total < UMBRAL_MAX-0.005) {
			umbral_total = umbral_total + 0.01;
			return 0;
		}
		else{
			umbral_total = UMBRAL_MIN;
			return -1;
		}
	}
}

void printCotas(cota_t *cotas, int nCotas){
	int i = 0;

	for(i = 0; i < nCotas; i++){
		printf("\t - Cota %d función %s, valor: %f, pendiente: %f\n",i,cotas[i].name, execute(cotas[i], pow(10,8), 0), execute(cotas[i], pow(10,8),  1));
	}

	printf("\n");
}
void mostrarTiempos(tAlgoritmo algoritmo, double tiempos[], cota_t cotas[]) {
	int n, i = 0, j;
	char print[261];

	printf("%12s\t%12s\t", "n", "t(n)");
	for (j = 0; j < 3; j++) {
		sprintf(print, "t(n)/%s", cotas[j].name);
		printf(" %17s\t", print);
	}
	printf("\n");

	for (n = algoritmo.ini; n <= algoritmo.fin; n = n * algoritmo.mult) {
		if (tiempos[i] < 500)
			printf("(*)%9d\t%12.3f\t\t%.8f\t\t%.8f\t\t%.8f\n", n, tiempos[i], tiempos[i]/execute(cotas[0], n, 0),
				   tiempos[i]/execute(cotas[1], n, 0), tiempos[i]/execute(cotas[2], n, 0));
		else
			printf("%12d\t%12.3f\t\t%.8f\t\t%.8f\t\t%.8f\n", n, tiempos[i], tiempos[i]/execute(cotas[0], n, 0),
				   tiempos[i]/execute(cotas[1], n, 0), tiempos[i]/execute(cotas[2], n, 0));
		i++;
	}
}