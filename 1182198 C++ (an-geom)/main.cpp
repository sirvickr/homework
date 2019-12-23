/*
Даны N точек на плоскости. Найти среди них точки, являющиеся вершинами фигуры, содержащей максимальное число заданных точек.
*/

#include <iostream>
#include <iomanip>

using namespace std;

struct point {
	double x = 0.0;
	double y = 0.0;
}

size_t count = 12;

point points[] = {
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

int main(int argc, char* argv[])
{
	/*if(argc != 2) {
		cout << "usage: " << argv[0] << " number" << endl;
		cout << "e.g. " << argv[0] << " 0x01020304" << endl;
		cout << "or   " << argv[0] << " 65535" << endl;
		return 0;
	}*/
	for(size_t i = 0; i < count; ++i) {
		cout << setw(7) << points[i].x << setw(7) << points[i].y << "\t" << endl;
	}
	return 0;
}
