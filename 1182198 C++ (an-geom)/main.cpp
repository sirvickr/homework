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
	static constexpr double eps = 1e-5;
	bool operator==(const point& pt) const {
		return fabs(x - pt.x) < eps && fabs(y - pt.y) < eps;
	}
	bool operator!=(const point& pt) const {
		return fabs(x - pt.x) >= eps && fabs(y - pt.y) >= eps;
	}
};

const size_t pointCount = 15;

const size_t paramCount = 10;

/*
Параметры фигуры (за основнуй вектор, с которым сравниваются осталтные вектора (AC, ... AN, ...), принят вектор AB):
'x' содержит соотношение длин векторов, 'y' содержит значение косинуса угла между векторами
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

// два набора тестовых данных (маленький и большой "кресты")
// точки A-L являются вершинами фигуры
#if 0
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
	{ 8.0, 9.0 }, // M
	{ 1.0, 1.0 }, // N
	{ 3.0, 2.0 }, // 0
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
	{  8.0,  9.0 }, // M
	{  1.0,  1.0 }, // N
	{  3.0,  2.0 }, // 0
};
#endif

bool vectorIsAppropriate(const point& param) {
	for(size_t k = 0; k < paramCount; ++k) 
		if (param == params[k]) 
			return true;
	return false;
}

int main(int argc, char* argv[]) {
//	for(size_t i = 0; i < pointCount; ++i)
	size_t i = 0;
	
//	size_t j = 1;
	for(size_t j = 0; j < pointCount; ++j) {

		if(i != j) {
			cout << "\t(" << j << ")" << endl; 
			point v1 = { points[j].x - points[i].x, points[j].y - points[i].y };
			double len1 = sqrt(v1.x * v1.x + v1.y * v1.y);
			
			for(size_t k = 2; k < pointCount; ++k) {
				
				point v2 = { points[k].x - points[i].x, points[k].y - points[i].y };
				
				double len2 = sqrt(v2.x * v2.x + v2.y * v2.y);
				double r = len2 / len1;
				double c = (v1.x * v2.x + v1.y * v2.y) / (len1 * len2);

				bool found = vectorIsAppropriate({ r, c });

				cout << "\t" << setw(2) << k << ") " << setw(7) << points[k].x << " " << setw(7) << points[k].y 
					<< "\tv:" << setw(7) << v2.x << setw(7) << v2.y << "\tlen:" << setw(9) << len2 
					<< " ratio " << setw(9) << r 
					<< "\tc: " << setw(9) << c << " found " << found
				<< endl;
			
			}
			break;
		}
	} // for(j)

	return 0;
}
