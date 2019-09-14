#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

/**
@mainpage Первое задание по технотреку. Решение квадратного уравнения

Состоит из следующих страниц:
- Function:
<ul>
	<li>ExpectDiskriminant
	<li>ExpectLinear
	<li>AisNull
	<li>BisNull
	<li>CisNull
	<li>AllNotNull
	<li>SquareSolve
	<li>PrintRoots
</ul>
*/
/**
@page ExpectDiscriminant
@section common ExpectDiscriminant
@brief вычисление дискриминанта

Функция для вычисления корня квадратного дискриминанта квадратного уравнения с заданными коэффициентами
@param a - первый коэффициент квадратного уравнения
@param b - второй коэффициент квадратного уравнения
@param c - третий коэффициент квадратного уравнения
@return корень дискриминанта, если тот равен или больше нуля, или -1 если меньше нуля
*/
float ExpectDiskriminant(const float* a, const float* b, const float* c){
	float kvdisk = (*b) * (*b) - 4*(*a)*(*c);
	if(kvdisk >= 0){
		return sqrt(kvdisk);
	}else{
		return -1;
	}
}

/*!
@page ExpectLinear
@section common ExpectLinear
@brief линейное уравнение

Функция для решения линейных уравнений с заданными коэффициентами
@param b - первый коэффициент линейного уравнения
@param с - второй коэффициент линейного уравнения
@param *root1 - адрес переменной, куда будет записано решение линейного уравнения с заданными коэффициентами (при наличии)
@return void
*/
void ExpectLinear(const float* b, const float* c, float* root1){
	assert (root1 != NULL);
	*root1 = -*c/(*b);
}

/**
@page AisNull
@section common AisNull
@brief A = 0

Функция для решения квадратного уравнения, у которого первый коэффициент равен 0
@param b - второй коэффициент линейного уравнения
@param с - третий коэффициент линейного уравнения
@param *root1 - адрес переменной, куда будет записано решение линейного уравнения с заданными коэффициентами (при наличии)
@return количество корней уравнения
*/
int AisNull(const float* b, const float* c, float* root1){
	assert (root1 != NULL);
	if(*b == 0){
		if(*c == 0){
			return -1;
		}else /*if(c != 0)*/ {
			return 0;
		}
	}else /*if (b != 0)*/ {
		if(*c == 0){
			*root1 = 0;
		}else /*if (c != 0)*/ {
			ExpectLinear(b, c, root1);
		}
		return 1;
	}
}

/**
@page BisNull
@section common BisNull
@brief B = 0, A != 0

Функция для решения квадратного уравнения, у которого второй коэффициент равен 0, а первый нет
@param a - первый коэффициент квадратного уравнения
@param с - третий коэффициент линейного уравнения
@param *root1 - адрес переменной, куда будет записан первый корень уравнения (при наличии)
@param *root2 - адрес переменной, куда будет записан второй корень уравнения (при наличии)
@return количество корней уравнения
*/
int BisNull(const float *a, const float *c, float* root1, float* root2){
	assert (x1 != NULL);
	assert (x2 != NULL);
	assert (x1 != x2);
	if(*c == 0){
		*root1 = 0;
		return 1;
	}else if(*c > 0){
		return -1;
	}else /*if(c < 0) */{
		*root1 = sqrt(-*c/(*a));
		*root2 = -*root1;
		return 2;
	}
}

/**
@page CisNull
section common CisNull
@brief C = 0, A*B != 0

Функция для решения квадратного уравнения, у которого третий коэффициент равен нулю, а первые два нет
@param a - первый коэффициент квадратного уравнения
@param b - второй коэффициент линейного уравнения
@param *root1 - адрес переменной, куда будет записан первый корень уравнения (при наличии)
@param *root2 - адрес переменной, куда будет записан второй корень уравнения (при наличии)
@return количество корней уравнения
*/
int CisNull(const float *a, const float *b, float* root1, float* root2){
	assert (x1 != NULL);
	assert (x2 != NULL);
	assert (x1 != x2);
	*root2 = 0;
	ExpectLinear(a, b, root1);
	return 2;
}

/**
@page AllNotNull
@section common AllNotNull
@brief A*B*C != 0

Функция для решения квадратного уравнения, у которого все коэффициенты ненулевые
@param a - первый коэффициент квадратного уравнения
@param b - второй коэффициент линейного уравнения
@param с - третий коэффициент линейного уравнения
@param *root1 - адрес переменной, куда будет записан первый корень уравнения (при наличии)
@param *root2 - адрес переменной, куда будет записан второй корень уравнения (при наличии)
return количество корней уравнения
*/
int AllNotNull(const float* a, const float *b, const float* c, float* root1, float* root2){
	assert (x1 != NULL);
	assert (x2 != NULL);
	assert (x1 != x2);
	float k = ExpectDiskriminant(a, b, c);
	if(ExpectDiskriminant < 0){
		return 0;
	}else /* if(ExpectDiskriminant > 0 */ {
		*root1 = (-(*b) + k)/(2*(*a));
		*root2 = (-(*b) - k)/(2*(*a));
		if(*root1 != *root2){
			return 2;
		}else /* if(root1 == root2) */ {
			*root2 = NAN;
			return 1;
		}
	}
}

/**
@page SquareSolve
@section common SquareSolve
@brief решение квадратного уравнения

Функция для решения квадратного уравнения, с заданными коэффициентами
@param a - первый коэффициент квадратного уравнения
@param b - второй коэффициент линейного уравнения
@param с - третий коэффициент линейного уравнения
@param *root1 - адрес переменной, куда будет записан первый корень уравнения (при наличии)
@param *root2 - адрес переменной, куда будет записан второй корень уравнения (при наличии)
return количество корней уравнения
*/
int SquareSolve(const float* a, const float* b, const float* c, float* root1, float* root2){
	assert (x1 != NULL);
	assert (x2 != NULL);
	assert (x1 != x2);
	if(*a == 0){
		return AisNull(b, c, root1);
	}else if(*b == 0) /* and (a != 0) */{
		return BisNull(a, c, root1, root2);
	}else if(*c == 0) /* and (a != 0) and (b != 0) */{
		return CisNull(a, b, root1, root2);
	}else /* a*b*c != 0 */ {
		return AllNotNull(a, b, c, root1, root2);
	}
}

/**
@page PrintRoots
@section common PrintRoots
@brief печать корней квадратного уравнения

Функция, выводящая в консоль корни квадратного уравнения с заданными коэффициентами и их количество
@param a - первый коэффициент квадратного уравнения
@param b - второй коэффициент линейного уравнения
@param с - третий коэффициент линейного уравнения
@param *root1 - адрес переменной, куда будет записан первый корень уравнения (при наличии)
@param *root2 - адрес переменной, куда будет записан второй корень уравнения (при наличии)
return количество корней уравнения
*/
void PrintRoots(const float* a, const float* b, const float* c, float* root1, float* root2){
	assert (x1 != NULL);
	assert (x2 != NULL);
	assert (x1 != x2);
	int count = SquareSolve(a, b, c, root1, root2);
	switch (count){

		case (-1):{
			printf("The equation has infinitely many roots\n");
			break;
		}

		case 0:{
			printf("The equation has no roots\n");
			break;
		}

		case 1:{
			printf("The equation has one root and that is %0.2f\n", *root1);
			break;
		}

		case 2:{
			printf("the equation has two roots and there are %0.2f and %0.2f\n", *root1, *root2);
			break;
		}

		default:{
			printf("ERROR. Function /Huilo/ returned impossible value. Line number 73\n");
			break;
		}
	}
}

int main(){
	float a = NAN, b = NAN, c = NAN;
	float root1 = NAN, root2 = NAN;
	scanf("%f %f %f", &a, &b, &c);
	PrintRoots(&a, &b, &c, &root1, &root2);
	return 0;
}
#ZernovN
#ZernovN
#ZernovN
