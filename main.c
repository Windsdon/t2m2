
// PVI:
// y1' = y2
// y2' = y1 + e^x
// Y(0) = [1, 0]

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void vec_times(const double *v, double x, double *va, int size){
	int i;
	for(i = 0; i < size; i++){
		va[i] = v[i] * x;
	}
}

void vec_add(const double *v1, const double *v2, double *va, int size){
	int i;
	for(i = 0; i < size; i++){
		va[i] = v1[i] + v2[i];
	}
}

// Executa o método de Euler Modificado com passo h
// para um sistema size X size da forma
// | Y' = F(Y, x)
// | Y(x0) = Y
// por steps iterações e retorna em y
double euler_mod(double *y, 
		void (*f)(const double *y, double x, double *fy), 
		double x0,  
		double h, 
		int steps,
		int size ){

	int i;
	double x = x0;


	for(i = 0; i < steps; i++){
		double x1 = x + h;
		double k1[size], k2[size], temp[size], yb[size];
		f(y, x, k1);
		vec_times(k1, h, temp, size);
		vec_add(y, temp, yb, size);
		f(yb, x1, k2);

		vec_add(k1, k2, temp, size);
		vec_times(temp, h/2, temp, size);
		vec_add(y, temp, y, size);

		x = x1;
	}

	return x;
}

// a função F(Y, x) para o PVI
void _f(const double *y, double x, double *fy) {
	fy[0] = y[1];
	fy[1] = y[0] + exp(x);
}

double expected(double x){
	return (exp(x) * (1 + 2*x) + 3*exp(-x))/4.0;
}

void line(double x, double vc, double ve) {
	printf("%13.10f\t%13.10f\t%13.10f\t%13.5e\n", x, vc, ve, fabs(ve - vc));
}

/*
*  Cria um vetor com os valores para montar a tabela de erros
*/
void make_vals(double x, double ym, double ye, double *vec) {
	vec = (double*) malloc(4 * sizeof(double));
	vec[0] = x;
	vec[1] = ym;
	vec[2] = ye;
	vec[3] = fabs(ye - ym);
}

typedef struct {
	int count;
	double h;
	double **points;
} erros;

int main() {
	int i, j, iters;
	erros **todosErros;

	todosErros = (erros*) malloc(4*sizeof(erros*));


	for(j = 0; j < 4; j++){
		double y[] = {1, 0};
		double x = 0;
		double h = 0.1/pow(2, j);
		double ye;
		erros *err = (erros*) malloc(sizeof(erros));
		todosErros[j] = err;

		printf("\n****\nh=%13f\n%13c\t%13c\t%13s\t%13c\n", h, 'x', 'y', "yc", 'e');

		ye = expected(x);
		line(x, y[0], ye);

		iters = (int) ceil(1.0/h);
		err->count = iters + 1;
		err->points = (double**) malloc((iters + 1) * sizeof(double*));
		err->h = h;

		make_vals(x, y[0], ye, err->points[0]);

		for(i = 0; i < iters; i++){
			x = euler_mod(y, _f, x, h, 1, 2);
			ye = expected(x);

			line(x, y[0], ye);			
			make_vals(x, y[0], ye, err->points[i + 1]);
		}

		printf("erros: h=%g, count=%d, points=%p\n", err->h, err->count, err->points);
	}

	printf("\n\nh\tE\tn\n");
	for(i = 0; i < 4; i++){
		erros *err = todosErros[i];
		double e, dif = 0, sum = 0;

		printf("start sum");

		for(j = 0; j < err->count; j++){
			printf("%g", err->points[j][2]);
			dif += pow(err->points[j][1] - err->points[j][2], 2);
			sum += pow(err->points[j][2], 2);
			printf("\n j = %d, dif = %g, sum = %g", j, dif, sum);
		}
		printf("end sum");

		e = sqrt(dif/sum);

		printf("%g %g %d\n", err->h, e, err->count - 1);

	}

	return 0;
}