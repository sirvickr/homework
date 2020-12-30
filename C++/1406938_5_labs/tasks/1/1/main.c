#include <stdio.h>
#include <math.h>

// точка входа программы
int main( int argc, char* argv[] )
{
	float x, y, z;
	printf("input x: ");
	scanf ("%f", &x);
	printf("input y: ");
	scanf ("%f", &y);
	printf("input z: ");
	scanf ("%f", &z);
	
	// сохраняем результаты промежуточных выражений, чтобы не вычислять их по 2 раза
	float x2plus4 = x * x + 4;
	float sinz = sin(z);

	float a = (1 + y) * ( x + y / x2plus4 / (pow(M_E, -x - 2) + 1 / x2plus4) );

	float b = 1 + cos(y - 2) / (x * x * x * x / 2 + sinz * sinz);
	
	printf("x %f y %f z %f:\na %f b %f\n", x, y, z, a, b);

	return 0;
}
