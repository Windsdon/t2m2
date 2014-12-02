/**
*   1º Trabalho de Métodos Numéricos para Engenharia II
*   Professor: Murilo F. Tomé
*/

#include<math.h>
#include<stdio.h>

// FUNCÕES AUXILIARES

// Multiplica cada elemento de um vetor por uma constante
void vec_times(const double *v, double x, double *va, int size){
	int i;
	for(i = 0; i < size; i++){
		va[i] = v[i] * x;
	}
}

// Soma dois vetores de mesmo tamanho
void vec_add(const double *v1, const double *v2, double *va, int size){
	int i;
	for(i = 0; i < size; i++){
		va[i] = v1[i] + v2[i];
	}
}

// FIM DAS FUNÇÕES AUXILIARES

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

// a função F(Y, x) para o PVI:
// y1' = y2
// y2' = y1 + e^x
// Y(0) = [1, 0]
void _f(const double *y, double x, double *fy) {
	fy[0] = y[1];
	fy[1] = y[0] + exp(x);
}

// Solução analítica para comparação
double expected(double x){
	return (exp(x) * (1 + 2*x) + 3*exp(-x))/4.0;
}

// Imprime uma linha de resultados
void line(double x, double vc, double ve) {
	printf("%13.10f\t%13.10f\t%13.10f\t%13.5e\n", x, vc, ve, fabs(ve - vc));
}


int main() {
	int i, j, iters;

	for(j = 0; j < 4; j++){
		double y[] = {1, 0};
		double x = 0;
		double h = 0.1/pow(2, j);
		double ye;

		printf("\n****\nh=%13f\n%13c\t%13c\t%13s\t%13c\n", h, 'x', 'y', "yc", 'e');

		ye = expected(x);
		line(x, y[0], ye);

		iters = (int) ceil(1.0/h);

		for(i = 0; i < iters; i++){
			x = euler_mod(y, _f, x, h, 1, 2);
			ye = expected(x);

			line(x, y[0], ye);
		}
	}

	return 0;
}