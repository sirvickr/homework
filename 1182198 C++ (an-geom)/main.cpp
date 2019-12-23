/*
Даны N точек на плоскости. Найти среди них точки, являющиеся вершинами фигуры, содержащей максимальное число заданных точек.
*/

#include <cmath>
#include <iostream>
#include <iomanip>
#include <map>
#include <vector>

using namespace std;

//#define TRACE_ON

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
"Шаблон" фигуры (за базовый вектор, с которым сравниваются остальные вектора (AC, ... AN, ...), принят вектор AB):
'x' содержит соотношение длин векторов, 'y' содержит значение косинуса угла между векторами
*/
const point_t params[paramCount] = {
//соотношение |  косинус
//       длин |     угла
	{ 2.23607,  0.447214 }, // [0] как AC относится к AB
	{       2,         0 }, // [1] как AD относится к AB (D, как и A, является опорной точкой внутреннего квадрата)
	{       3,         0 }, // [2] как AE относится к AB
	{ 3.60555,   -0.5547 }, // [3] как AF относится к AB
	{ 2.82843, -0.707107 }, // [4] как AG относится к AB (G, как и A, является опорной точкой внутреннего квадрата)
	{ 3.60555,  -0.83205 }, // [5] как AH относится к AB
 	{       3,        -1 }, // [6] как AI относится к AB
 	{       2,        -1 }, // [7] как AJ относится к AB (J, как и A, является опорной точкой внутреннего квадрата)
	{ 2.23607, -0.894427 }, // [8] как AK относится к AB
	{       1,        -0 }  // [9] как AL относится к AB
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
	// каждый из десяти бит маски - это логический флаг, означающий удобвлетворение соответствующему элменту "шаблона" фигуры
	const size_t paramMask = (1 << paramCount) - 1;
	figures_t figures;
	size_t pointCount = points.size();
	// внешний цикл [i] - перебираем все точки и рассматриваем их как основание базового вектора
	for(size_t i = 0; i < pointCount; ++i) {
		#ifdef TRACE_ON
		cout << "#### " << i << " ############################################################" << endl; 
		#endif
		if(!points[i].exludeBase) {
	
			// для каждой из точек, интерпретируя её как основание базового вектора,
			for(size_t j = 0; j < pointCount; ++j) {
				// перебираем все остальные точки, рассматривая их как вершину базового вектора
				point_t v1 = { points[j].x - points[i].x, points[j].y - points[i].y };
				double len1 = sqrt(v1.x * v1.x + v1.y * v1.y);
				
				if(j != i && !points[j].exludeTop && len1 > 0) {
					#ifdef TRACE_ON
					cout << "\t(" << j << ")" << endl; 
					#endif
					size_t mask = 0;
					size_t vertexIndex = 2;
					// запоминаем вершины:
					vector<size_t> vertexes(vertexCount); // все
					vector<size_t> vBaseExcludes; // вершины внутреннего квадрата фигуры
					vBaseExcludes.reserve(3);
					vector<size_t> vTopExcludes; // вершины базовых векторов, которые будем исключать в случае успеха
					vTopExcludes.reserve(3);
					// для каждого базового вектора (т.е. вектора c i-м основанием и j-й вершиной),
					// ищем "вокруг него" фигуру, основываясь заранее просчитанном на образце params,
					// который содержит угловые смещения и пропорции всех вершин относительно базового вектора 
					for(size_t k = 0; k < pointCount; ++k) {

						if(k != i && k != j) {

							point_t v2 = { points[k].x - points[i].x, points[k].y - points[i].y };
							
							double len2 = sqrt(v2.x * v2.x + v2.y * v2.y);
							// отношение длины текущего проверяемого вектора (c i-м основанием и k-й вершиной) к текущему базовому (c i-м основанием и j-й вершиной)
							double ratio = len2 / len1; 
							if(len2 > 0.0) {
								double cosin = (v1.x * v2.x + v1.y * v2.y) / (len1 * len2); // косинус угла между этими векторами

								size_t index = vectorIsAppropriate( { ratio, cosin } );
								if(index < paramCount) {
									// эти точки принадлежат внутреннему квадрату фигуры,
									// и если фигура относительно точки[i] будет завершена, их можно исключить,
									// во избежание лишних итераций (к тому же фигура полностью находится относительно любой из них)
									if(1 == index || 4 == index || 7 == index) {
										#ifdef TRACE_ON
										cout << "{" << k << "}";
										#endif
										vBaseExcludes.push_back(k);
									} else if(paramCount - 1 == index) {
										#ifdef TRACE_ON
										cout << "<" << k << ">";
										#endif
										vTopExcludes.push_back(k);
									}
									mask |= (1 << index);
									vertexes[vertexIndex++] = k;
								}
								#ifdef TRACE_ON
								cout << "\t" << setw(2) << k << ") "// << setw(7) << points[k].x << " " << setw(7) << points[k].y 
								//	<< "\tv:" << setw(7) << v2.x << setw(7) << v2.y << "\tlen:" << setw(9) << len2 
									<< " ratio " << setw(9) << ratio 
									<< "\tcosin: " << setw(9) << cosin << " " << boolalpha << (index < paramCount) << " (" << index << ") " << hex << mask << "/" << paramMask << dec;
								#endif
								if(paramMask == mask) {
									cout << " FIGURE DETECTED";
									auto it = figures.find(points[i]);
									if(it == figures.end()) {
										cout << " its vertexes:";
										vertexes[0] = i;
										vertexes[1] = j;
										for(auto item : vertexes)
											cout << " " << item;
										cout << " base:";
										for(auto index : vBaseExcludes) {
											//points[index].exludeBase = true;
											for(size_t x = 0; x < pointCount; ++x) {
												if(points[x] == points[index]) { // сравниваем по координатам, поскольку разные точки могут "лежать" одна на другой 
													cout << " " << x;
													points[x].exludeBase = true;
												}
											}
										}
										cout << " top:";
										for(auto index : vTopExcludes) {
											//points[index].exludeTop = true;
											for(size_t x = 0; x < pointCount; ++x) {
												if(points[x] == points[index]) { // сравниваем по координатам, поскольку разные точки могут "лежать" одна на другой
													cout << " " << x;
													points[x].exludeTop = true;
												}
											}
										}
										figures[points[i]] = vertexes;
									}
									cout << endl;
									break;
								}
								#ifdef TRACE_ON
								cout << endl;
								#endif
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
