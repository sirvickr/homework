/*
Даны N точек на плоскости. Найти среди них точки, являющиеся вершинами фигуры, содержащей максимальное число заданных точек.
*/

#include <cmath>
#include <iostream>
#include <iomanip>
#include <map>
#include <vector>

using namespace std;

struct point_t {
	double x;
	double y;
	// флаг означает, что эта точка является вершиной внутреннего квадрата фигуры
	// и служит для того, чтобы не искать фигуру по каждой из 4 вершин
	bool exludeBase; 
	bool exludeTop; 
	static constexpr double eps = 1e-5;
	bool operator==(const point_t& pt) const {
		return fabs(x - pt.x) < eps && fabs(y - pt.y) < eps;
	}
	bool operator!=(const point_t& pt) const {
		return fabs(x - pt.x) >= eps && fabs(y - pt.y) >= eps;
	}
	bool operator<(const point_t& pt) const {
		if(x < pt.x)
			return true;
		else if(fabs(x - pt.x) < eps) {
			if(y < pt.y) {
				return true;
			}
		}
		return false;
	}
};

using figures_t = map<point_t, vector<size_t>>;

// количество вершин искомой фигуры
const size_t vertexCount = 12;
// количество параметров (в вершины включены в базовый вектор)
const size_t paramCount = vertexCount - 2;

/*
Параметры фигуры (за базовый вектор, с которым сравниваются осталтные вектора (AC, ... AN, ...), принят вектор AB):
'x' содержит соотношение длин векторов, 'y' содержит значение косинуса угла между векторами
*/
const point_t params[paramCount] = {
	{ 2.23607,  0.447214 }, // [0] AC to AB
	{       2,         0 }, // [1] AD to AB (D, как и A, является опорной точкой внутреннего квадрата)
	{       3,         0 }, // [2] AE to AB
	{ 3.60555,   -0.5547 }, // [3] AF to AB
	{ 2.82843, -0.707107 }, // [4] AG to AB (G, как и A, является опорной точкой внутреннего квадрата)
	{ 3.60555,  -0.83205 }, // [5] AH to AB
 	{       3,        -1 }, // [6] AI to AB
 	{       2,        -1 }, // [7] AJ to AB (J, как и A, является опорной точкой внутреннего квадрата)
	{ 2.23607, -0.894427 }, // [8] AK to AB
	{       1,        -0 }  // [9] AL to AB
};

// набор тестовых данных (две фигуры и дополнительные точки)
// точки A,B,C,D,E,F,G,H,I,J,K,L являются вершинами большого "креста"
// точки A1,B1,C1,D1,E1,F1,G1,H1,I1,J1,K1,L1 являются вершинами маленького "креста"
vector<point_t> points = {
	{  2.0,  6.0, false, false }, //[0]  S

	{  5.0,  4.0, false, false }, //[1]  A *

	{  7.0,  6.0, false, false }, //[2]  P

	{  3.0,  4.0, false, false }, //[3]  B *
	{  3.0,  8.0, false, false }, //[4]  C *
	{  5.0,  8.0, false, false }, //[5]  D *
	{  5.0, 10.0, false, false }, //[6]  E *
	{  9.0, 10.0, false, false }, //[7]  F *
	{  9.0,  8.0, false, false }, //[8]  G *
	{ 11.0,  8.0, false, false }, //[9]  H *
	{ 11.0,  4.0, false, false }, //[10] I *
	{  9.0,  4.0, false, false }, //[11] J *
	{  9.0,  2.0, false, false }, //[12] K *
	{  5.0,  2.0, false, false }, //[13] L *
	{  8.0,  9.0, false, false }, //[14] M
	{  1.0,  1.0, false, false }, //[15] N
	{  3.0,  2.0, false, false }, //[16] 0

	{  2.0,  2.0, false, false }, //[17] A1 *
	{  1.0,  2.0, false, false }, //[18] B1 *
	{  1.0,  4.0, false, false }, //[19] C1 *
	{  2.0,  4.0, false, false }, //[20] D1 *
	{  2.0,  5.0, false, false }, //[21] E1 *
	{  4.0,  5.0, false, false }, //[22] F1 *
	{  4.0,  4.0, false, false }, //[23] G1 *
	{  5.0,  4.0, false, false }, //[24] H1 *
	{  5.0,  2.0, false, false }, //[25] I1 *
	{  4.0,  2.0, false, false }, //[26] J1 *
	{  4.0,  1.0, false, false }, //[27] K1 *
	{  2.0,  1.0, false, false }, //[28] L1 *

	{ 12.0, 11.0, false, false }, //[29] R
};

size_t vectorIsAppropriate(const point_t& param) {
	for(size_t k = 0; k < paramCount; ++k) 
		if (param == params[k]) 
			return k;
	return paramCount;
}

int main(int argc, char* argv[]) {
	figures_t figures;
	size_t pointCount = points.size();
//	size_t i = 1; // A;
	for(size_t i = 0; i < pointCount; ++i) {
		cout << "#### " << i << " ############################################################" << endl; 
		if(!points[i].exludeBase) {
	
			//size_t j = 1;
			for(size_t j = 0; j < pointCount; ++j) {

				point_t v1 = { points[j].x - points[i].x, points[j].y - points[i].y };
				double len1 = sqrt(v1.x * v1.x + v1.y * v1.y);
				
				if(j != i && !points[j].exludeTop && len1 > 0) {
					cout << "\t(" << j << ")" << endl; 

					size_t mask = 0;
					size_t vertexIndex = 2;
					vector<size_t> vertexes(vertexCount);
					vector<size_t> vBaseExcludes;
					vBaseExcludes.reserve(3);
					vector<size_t> vTopExcludes;
					vTopExcludes.reserve(3);

					for(size_t k = 0; k < pointCount; ++k) {

						if(k != i && k != j) {

							point_t v2 = { points[k].x - points[i].x, points[k].y - points[i].y };
							
							double len2 = sqrt(v2.x * v2.x + v2.y * v2.y);
							double ratio = len2 / len1;
							double denominator = len1 * len2;
							if(len2 > 0.0) {
								double cosin = (v1.x * v2.x + v1.y * v2.y) / denominator;

								const size_t paramMask = (1 << paramCount) - 1;
								size_t index = vectorIsAppropriate( { ratio, cosin } );
								if(index < paramCount) {
									// эти точки принадлежат внутреннему квадрату фигуры,
									// и если фигура относительно точки[i] будет завершена, их можно исключить,
									// во избежание лишних итераций (к тому же фигура полностью находится относительно любой из них)
									if(1 == index || 4 == index || 7 == index) {
										cout << "{" << k << "}";
										vBaseExcludes.push_back(k);
									} else if(paramCount - 1 == index) {
										cout << "<" << k << ">";
										vTopExcludes.push_back(k);
									}
									mask |= (1 << index);
									vertexes[vertexIndex++] = k;
								}
								#if 1
								cout << "\t" << setw(2) << k << ") "// << setw(7) << points[k].x << " " << setw(7) << points[k].y 
								//	<< "\tv:" << setw(7) << v2.x << setw(7) << v2.y << "\tlen:" << setw(9) << len2 
									<< " ratio " << setw(9) << ratio 
									<< "\tcosin: " << setw(9) << cosin << " " << boolalpha << (index < paramCount) << " (" << index << ") " << hex << mask << "/" << paramMask << dec;
								#endif
								if(paramMask == mask) {
									cout << " FIGURE DETECTED";
									auto it = figures.find(points[i]);
									if(it == figures.end()) {
										cout << " new:";
										for(auto index : vBaseExcludes)
											points[index].exludeBase = true;
										for(auto index : vTopExcludes)
											points[index].exludeTop = true;
										vertexes[0] = i;
										vertexes[1] = j;
										for(auto item : vertexes)
											cout << " " << item;
										figures[points[i]] = vertexes;
									}
									cout << endl;
									break;
								}
								cout << endl;
							} // if(denominator > 0)

						} // for(k)
					
					}
					//break;
				}
			} // for(j)

		}

	} // for(i)

	return 0;
}
