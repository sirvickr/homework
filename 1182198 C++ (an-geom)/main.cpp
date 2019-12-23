/*
Даны N точек на плоскости. Найти среди них точки, являющиеся вершинами фигуры, содержащей максимальное число заданных точек.
*/

#include <cmath>
#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;

struct point {
	double x;
	double y;
};

const size_t pointCount = 12;

const size_t paramCount = 10;
/* параметры фигуры:
x содержит соотношение длин векторов,
y содержит значение косинуса угла между векторами
*/
const point params[paramCount] = {
	{ 2.23607,  0.447214 },
	{       2,         0 },
	{       3,         0 },
	{ 3.60555,   -0.5547 },
	{ 2.82843, -0.707107 },
	{ 3.60555,  -0.83205 },
 	{       3,        -1 },
 	{       2,        -1 },
	{ 2.23607, -0.894427 },
	{       1,        -0 }
};

#if 1
point points[pointCount] = {
	{ 2.0, 2.0 }, // A
	{ 1.0, 2.0 }, // B
	{ 1.0, 4.0 }, // C
	{ 2.0, 4.0 }, // D
	{ 2.0, 5.0 }, // E
	{ 4.0, 5.0 }, // F
	{ 4.0, 4.0 }, // G
	{ 5.0, 4.0 }, // H
	{ 5.0, 2.0 }, // I
	{ 4.0, 2.0 }, // J
	{ 4.0, 1.0 }, // K
	{ 2.0, 1.0 }, // L
};
#else
point points[pointCount] = {
	{  5.0,  4.0 }, // A
	{  3.0,  4.0 }, // B
	{  3.0,  8.0 }, // C
	{  5.0,  8.0 }, // D
	{  5.0, 10.0 }, // E
	{  9.0, 10.0 }, // F
	{  9.0,  8.0 }, // G
	{ 11.0,  8.0 }, // H
	{ 11.0,  4.0 }, // I
	{  9.0,  4.0 }, // J
	{  9.0,  2.0 }, // K
	{  5.0,  2.0 }, // L
};
#endif

bool compare(double a, double b) {
	const double eps = 1e-5;
//	cout << "compare(" << a << ", " << b << "): " << fabs(a - b) << ";";
	return fabs(a - b) < eps;
}

int main(int argc, char* argv[])
{
	//for(size_t i = 0; i < pointCount; ++i)
	size_t i = 0;
	
	point v1 = { points[1].x - points[0].x, points[1].y - points[0].y };
	double len1 = sqrt(v1.x * v1.x + v1.y * v1.y);
	
	for(size_t j = 2; j < pointCount; ++j) {
		point v2 = { points[j].x - points[i].x, points[j].y - points[i].y };
		double len2 = sqrt(v2.x * v2.x + v2.y * v2.y);
		double r = len2 / len1;
		double c = (v1.x * v2.x + v1.y * v2.y) / (len1 * len2);
		bool found = false;
		for(size_t k = 0; k < paramCount; ++k) {
			if (compare(r, params[k].x) && compare(c, params[k].y)) {
				found = true;
				break;
			}
		}
		cout << setw(2) << j << ") " << setw(7) << points[j].x << " " << setw(7) << points[j].y 
			<< "\tv:" << setw(7) << v2.x << setw(7) << v2.y << "\tlen:" << setw(9) << len2 
			<< " ratio " << setw(9) << r 
			<< "\tc: " << setw(9) << c << " found " << found
		<< endl;
	}

	return 0;
}
