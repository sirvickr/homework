/* 84 (в) Даны натуральное n, действительное х. Вычислить сумму n слагаемых sin(x) + sin(sin(x) + ...
 */

#include <stdio.h>
#include <math.h>

// точка входа программы
int main( int argc, char* argv[] )
{
	int n;
	double x;
	printf(" input n: ");
	scanf ("%d", &n);
	printf(" input x: ");
	scanf ("%lf", &x);
	if(n > 0) {
		double result = 0;
		for(int i = 0; i < n; i++) {
			printf(" x: %lf -> ", x);
			x = sin(x);
			result += x;
			printf("%lf (sum = %lf)\n", x, result);
		}
		printf(" result: %lf\n", result);
	}

	return 0;
}
