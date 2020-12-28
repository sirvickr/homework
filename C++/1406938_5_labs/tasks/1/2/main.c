/* Даны действительные числа x , y . Определить, принадлежит ли точка с координатами x , y 
 * заштрихованной части плоскости (рис. 2д).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef struct {
	float X;
	float Y;
} Point;

int IsPointInsidePolygon(Point *p, int count, Point point) {
	int result = 0;
	int j = count - 1;
	for (int i = 0; i < count; i++) {
		if ( (p[i].Y < point.Y && p[j].Y >= point.Y || p[j].Y < point.Y && p[i].Y >= point.Y) &&
			(p[i].X + (point.Y - p[i].Y) / (p[j].Y - p[i].Y) * (p[j].X - p[i].X) < point.X) )
		result = !result;
		j = i;
	}
	return result;
}

// точка входа программы
int main( int argc, char* argv[] )
{
	#define VERTEX_COUNT 4
	Point shape[VERTEX_COUNT] = {
		{  0.0f,  1.0f },
		{  0.5f,  0.0f },
		{  0.0f, -1.0f },
		{ -0.5f,  0.0f },
	};
	Point point;
	printf("input x: ");
	scanf ("%f", &point.X);
	printf("input y: ");
	scanf ("%f", &point.Y);
	
	int in = IsPointInsidePolygon(shape, VERTEX_COUNT, point);
	
	printf("point (%f %f) is ", point.X, point.Y);
	if(in)
		printf("inside");
	else
		printf("outside");
	printf(" the shape\n");

	return 0;
}
